/*
 * TProtocol.cpp
 *
 *  Created on: Feb 25, 2021
 *      Author: energia
 */

#include <iterator>

#include "TProtocol.hpp"
#include "TComProtocol.hpp"
#include "TCommon.hpp"
#include "TApplication.hpp"

namespace app {

//----------------------------------------------------------
/*!
 * @param inHndl Указатель на хендл порта RS485
 */
TProtocol::TProtocol(UART_HandleTypeDef* inHndl) : TStmUart (inHndl)
{
	mDataRx.reserve(defDataSize) ;	// Из-за кривости стандартных аллокаторов для STM задаём максимальный размер контейнеров
	mDataTx.reserve(defDataSize) ;
}
//----------------------------------------------------------
/*!
 * @return При получении символов завершения сообщения возвращается true
 */
bool TProtocol::receiveByte ()
{
	bool retValue { false } ;
	uint32_t static timeReceiveByte ;

	if ((HAL_GetTick() - timeReceiveByte) > 100) mDataRx.clear() ;	// Если за 100 мСек не поступило ни одного байта, то чистим буфер приёма
	timeReceiveByte = HAL_GetTick() ;

	switch (mDataRx.size()) {
	  case defStartBytePos :		// Ждём стартовый байт протокола
		if (mByte == defStartByte) {
			mDataRx.push_back (mByte) ;
			mPrevByte = 0 ;
		}
	  break;

	  case defDstPos: {				// Проверяем адресата команды
		uint16_t address = application -> mSysInfo.getAddr () ;
		if (mByte == 0 || mByte == static_cast <uint8_t> (address)) mDataRx.push_back (mByte) ;
		  else mDataRx.clear() ;
	  }
	  break ;

	  case defSrcPos:				// Хрен его знает куда отправлять ответ.
		if (mByte == 0) mDataRx.clear() ;
		  else mDataRx.push_back (mByte) ;
	  break ;

	  case defTypePos:				// Для адресных команд проверяем адрес
		switch (static_cast <comProtocol::defCommand> (mByte)) {
		  case comProtocol::defCommand::cmdFE:	// Для безадресных команд всё пропускаем
		  case comProtocol::defCommand::cmd00:
		  case comProtocol::defCommand::cmd01:
		  case comProtocol::defCommand::cmdA0:
			mDataRx.push_back (mByte) ;
		  break;

		  default:					// для адресной команды поле DST = 0
			if (mDataRx.at (defDstPos) == 0x00) mDataRx.clear() ;
			  else mDataRx.push_back (mByte) ;
		  break;
		}
	  break ;

	  default:
		switch (mByte) {			// вычищаем дублирующие служебные символы
		  case defStartByte:
			if (mPrevByte != defStartByte) {
				mPrevByte = defStartByte ;
				mDataRx.push_back (mByte) ;
			}
			  else mPrevByte = 0 ;
		  break;

		  case defEndByte:
			if (mPrevByte != defEndByte) {
				mPrevByte = defEndByte ;
				mDataRx.push_back (mByte) ;
			}
			  else mPrevByte = 0 ;
		  break;

		  default:
			mPrevByte = 0 ;
			mDataRx.push_back (mByte) ;
		  break;
		}

	  break;
	}

	if (mByte == defStopByte && mDataRx.size() > 1)
		if (mDataRx.at (mDataRx.size() - 2) == defEndByte)
			retValue = true ;

	return retValue ;
}
//----------------------------------------------------------
/*!
 * @param inByte Принятый байт
 * @return При получении символов завершения сообщения возвращается true
 */
bool TProtocol::receiveByte (uint8_t inByte) {
	mByte = inByte ;
	return receiveByte () ;
}
//----------------------------------------------------------
/*!
 * @return При успешном запуске возвращается true ;
 */
bool TProtocol::startReceiveByte ()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);		// !!! Поменять нахрен !!!

	return receiveIT (&mByte, sizeof (mByte)) ;
}
//----------------------------------------------------------
/*!
 * @return При успешном запуске возвращается true ;
 * \todo Передалать на нормальную работу с DMA
 */
bool TProtocol::makeAnswer ()
{
	bool retValue  { false } ;
	comProtocol::defCommand typeCommandAnswer { comProtocol::defCommand::cmdUnknown } ;	// Команда для формирования ответа (не всегда полученная
	comProtocol::typeErr typeError { comProtocol::typeErr::noError } ;

	if (mDataRx.size() < 9) {
		typeCommandAnswer = comProtocol::defCommand::cmdF0 ;
		typeError = comProtocol::typeErr::errFormat ;
	}
	  else {											// буфер сформирован правильно
		if (mDataRx.at (defVersionPos) != defVersion) {	// Проверяем версию протокола взаимодействия
			typeCommandAnswer = comProtocol::defCommand::cmdF0 ;
			typeError = comProtocol::typeErr::errVersion ;
		}
		  else {										// Всё вроде как нормально и проверяем CRC
			uint16_t checkCRC = getCRC (mDataRx) ;
			uint16_t receiveCRC = *(mDataRx.end() - 3) ;
			receiveCRC = receiveCRC << 8 ;
			receiveCRC = receiveCRC + *(mDataRx.end() - 4) ;
			if (checkCRC != receiveCRC) {
				typeCommandAnswer = comProtocol::defCommand::cmdF0 ;
				typeError = comProtocol::typeErr::errCRC ;
			}
			  else typeCommandAnswer = static_cast <comProtocol::defCommand> (mDataRx.at (defTypePos)) ;	// Всё хорошо и нормально обрабатываем команду
		  }
	  }

	auto protocol = comProtocol::TComProtocol::create (typeCommandAnswer) ;
	if (protocol.get() != nullptr) {						// Если возвращается nullptr, то такую команду вообще не обрабатывает, т.к. её отродясь нет
		typeBufData dataTx ;									// Формируем промежуточный буфер для ответа

		dataTx.push_back(defStartByte) ;						// Формируем заголовок буфера
		dataTx.push_back (mDataRx.at (defSrcPos)) ;
		dataTx.push_back (mDataRx.at (defDstPos)) ;
		dataTx.push_back (defVersion) ;
		protocol -> process(mDataRx, dataTx, typeError) ;		// Формирование ответа на команду
		for (uint8_t i = 0; i < 4; ++i) dataTx.push_back (0) ;  // Забиваем буфер нулями, что бы посчитать CRC

		mDataTx.clear() ;										// Формируем буфер для передачи, т.е. добавляем CRC и дублируем служебные символы
		mDataTx.push_back(defStartByte) ;
		for (auto it = dataTx.begin() + 1 ; it != dataTx.end() - 4; ++it) checkControlByte (*it) ;
		uint16_t crc = getCRC (dataTx) ;						// Формируем CRC
		checkControlByte (crc & 0xFF) ;
		checkControlByte (crc >> 8) ;
		mDataTx.push_back(defEndByte) ;
		mDataTx.push_back(defStopByte) ;

//	stopDMA() ;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);		// !!! Поменять нахрен !!!

		retValue = transmit (mDataTx) ;						// !!! Переделать на DMA

	mDataRx.clear() ;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);	// !!! Поменять нахрен !!!
	}
	mDataRx.clear() ;
	return retValue ;
}
//----------------------------------------------------------
/*!
 * @param inBuf	Ссылка на сообщение для которого нужно рассчитать CRC
 * @return Рассчитанная CRC
 */
uint16_t TProtocol::getCRC (std::vector<uint8_t>& inBuf)
{
    uint8_t i;
    uint16_t wCrc { 0 } ;					// CRC с вектором инициализации 0x00
    if (inBuf.size() != 0) {
    	for (auto it = inBuf.begin() + 1; it != inBuf.end () - 4; ++it) {
    		uint8_t byte = *it ;
    		wCrc ^= byte << 8;					// основной цикл вычисления CRC
    		for (i=0; i < 8; i++)
    			wCrc = wCrc & 0x8000 ? (wCrc << 1) ^ 0x1021 : wCrc << 1;
    	}
    }
    return wCrc ;
}
//----------------------------------------------------------
/*!
 * @return Рассчитанная CRC
 */
uint16_t TProtocol::setRxCRC ()
{
	uint16_t retValue { 0} ;

	if (mDataRx.size() > 8) {
		retValue = getCRC (mDataRx) ;
		*(mDataRx.end() - 3) = retValue >> 8 ;
		*(mDataRx.end() - 4) = retValue & 0xFF ;
	}
	getCRC (mDataRx) ;


	return getCRC (mDataRx) ;
}
//----------------------------------------------------------
/*!
 * @param inByte Байт добавляемый в буфер для передачи
 */
void TProtocol::checkControlByte (uint8_t inByte)
{
	mDataTx.push_back(inByte) ;
	if (inByte == defStartByte || inByte == defEndByte) mDataTx.push_back(inByte) ;
}
//----------------------------------------------------------
} /* namespace app */
