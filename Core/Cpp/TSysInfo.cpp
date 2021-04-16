/*
 * TSysInfo.cpp
 *
 *  Created on: 11 мар. 2021 г.
 *      Author: AAL
 */

//#include "exception"
#include "vector"
#include "algorithm"
#include "iterator"

#include "TSysInfo.hpp"
#include "TState.hpp"
#include "TApplication.hpp"

namespace app {
//---------------------------------------------------------
TSysInfo::TSysInfo() : stm::TStmSpi (common::defPrtSpiFlash)
{
	uint8_t alarmData [defNumByte] ;						// Массив для поиска актуальной записи о важных событиях

	mKey0.reserve (common::defKeyNumber) ;
	mKey1.reserve (common::defKeyNumber) ;
	mKey2.reserve (common::defKeyNumber) ;
	mKey3.reserve (common::defKeyNumber) ;

	mAlarmData [apFlag] = 0 ;
	try {
		if (!readKeys(0)) throw 0 ;							// Читаем ключи
		if (!readKeys(1)) throw 0 ;
		if (!readKeys(2)) throw 0 ;
		if (!readKeys(3)) throw 0 ;

		if (!sendCmd (defFlashCmd::cmdD7)) throw 0  ;
		if (!sendCmd (defFlashCmd::cmd0B, defFlashPageAlarm, 0)) throw 0 ; // Ищем последнюю запись а важных событиях
		if (!read (alarmData, defNumByte)) throw 0 ;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

		uint32_t id { 0 } ;
		uint32_t sizeBuf = (defNumByte / apCount) * apCount ; //Убираем лишние байты
		for (; id < sizeBuf; id = id + apCount) if (mAlarmData [id] == 0xFF) break ;
		if (id == 0) {												// Страница пустая, т.е. с ней не работали
			for (uint32_t i = 0; i < apCount; ++i) mAlarmData [i] = 0 ;
			mAlarmPos = 0 ;
		}
		  else {													// Всё супер и заполняем флаги
			mAlarmData [apFlag] = 0 ;
			mAlarmData [apReaderAlarm] = alarmData [id + apReaderAlarm] ;
			mAlarmData [apButtonAlarm] = alarmData [id + apButtonAlarm] ;
			mAlarmData [apLockAlarm] = alarmData [id + apLockAlarm] ;
			mAlarmData [apDoorAlarm] = alarmData [id + apDoorAlarm] ;
			mAlarmData [apUnitAlarm] = alarmData [id + apUnitAlarm] ;
			mAlarmPos = id + apCount ;
		  }
	}
	  catch (...) {
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
		  for (uint32_t i = 0; i < apCount; ++i) mAlarmData [i] = 0 ;
		  std::shared_ptr <TState> ptrStateUnit (new TState (common::internalFlash, common::defState::flashError)) ;
		  app::application -> setQueueTxElement (ptrStateUnit) ;
	  }
}
//---------------------------------------------------------
/*!
 * @param inCmd Команда для работы с чипом
 * @param inPage Номер страницы
 * @param inAddr Адрес
 * @return При успешном выполнении возвращает true
 * \attention Правильность передаваемый параметров я не проверяю!!!
 * \todo Переделать управление пином PB12
 * \todo Сделать обработку ошибок
 * \todo Сделать универсальную процедуру формирования адреса
 */
bool TSysInfo::sendCmd (defFlashCmd inCmd, const uint16_t inPage, const uint16_t inAddr)
{
	bool retValue { false } ;
	uint8_t cmd [8] { 0 } ;				// Буфер для передачи команды

	cmd [0] = static_cast <uint8_t> (inCmd) ;

	try {
		switch (inCmd) {
		  case defFlashCmd::cmd9F: {
			uint8_t bufRead [5] { 0 } ;

			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
			if (transmit (cmd, 1) != true) throw 0 ;
			if (read(bufRead, sizeof (bufRead)) != true) throw 0 ;
			if (bufRead [0] != defAdestoID || bufRead [1] != defDeviceID) throw 0 ;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
			retValue = true ;
		  }
		  break;

		  case defFlashCmd::cmdD7: {
			uint8_t bufRead [2] { 0 } ;
			uint32_t isReady = 0xFFFF ;						// Счётчик блокировки ухода в бесконечный цикл

			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
			if (transmit (cmd, 1) != true) throw 0 ;
			do {
				if (read(bufRead, sizeof (bufRead)) != true) throw 0 ;
			} while (!(bufRead [0] & defStatusBit) && --isReady != 0) ;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

			if (isReady == 0) throw "JOPA" ; 					// Если попали сюда, то жопа
			retValue = true ;
		  }
		  break ;

		  case defFlashCmd::cmd0B: {
			if (inPage > defNumPage || inAddr > defNumByte) throw 0 ;

			cmd [1] = (inPage >> 6) & 0x3F ;				// Формируем необходимый адрес
			cmd [2] = ((inPage << 2)  & 0xFC) | ((inAddr >> 8) & 0x03) ;
			cmd [3] = inAddr & 0xFF ;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); // !!! Поднять PIN нужно после чтения данных
			if (transmit (cmd, 5) != true) throw 0 ; 		// !!! Возможный косяк!!! PIN не притягиваем в V т.к. нам ещё нужно прочитать данные
			retValue = true ;
		  }
		  break ;

		  case defFlashCmd::cmd81 : {
			cmd [1] = (inPage >> 6) & 0x3F ;				// Формируем необходимый адрес
			cmd [2] = (inPage << 2)  & 0xFC ;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
			if (transmit (cmd, 4) != true) throw 0 ;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
			retValue = true ;
		  }
		  break ;

		  case defFlashCmd::cmd85 : {
			cmd [1] = (inPage >> 6) & 0x3F ;				// Формируем необходимый адрес
			cmd [2] = ((inPage << 2)  & 0xFC) | ((inAddr >> 8) & 0x03) ;
			cmd [3] = inAddr & 0xFF ;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);  // !!! Поднять PIN нужно после записи данных
			if (transmit (cmd, 4) != true) throw 0 ;		// Отправляем команду, а читаем данные в другом месте
			retValue = true ;
		  }
		  break ;

		  default:
		  break;
		}
	}
	  catch (...) {
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
		  retValue = false ;
	  }

	return retValue ;
}
//---------------------------------------------------------
/*!
 * @return При успешной проверке чипа возвращает true
 * Проверка системной информации в чипе (ID производителя и размер)
 */
bool TSysInfo::checkFlash ()
{
	return sendCmd (defFlashCmd::cmd9F) ;
}
//---------------------------------------------------------
/*!
 * @return Адрес блока записанный на флэш. При ошибке возвращает 0.
 * \todo Приделать правильную обработку ошибок
 * \todo Переделать управление пином PB12
 */
uint8_t TSysInfo::getAddr ()
{
	uint8_t retValue { 0 } ;

	try {
		uint8_t bufRead [2] { 0 } ;

		if (!sendCmd (defFlashCmd::cmdD7)) throw 0 ;
		if (!sendCmd (defFlashCmd::cmd0B, defFlashPageSysInfo, defFlashAddrSysAddrUnit)) throw 0 ;
		if (!read (bufRead, 2)) throw 0 ;
		retValue = bufRead [0] ;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);	// Притягиваем к V PIN управления
	}
	  catch (...) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	  }

	return retValue ;
}
//---------------------------------------------------------
uint16_t TSysInfo::getSN ()
{
	uint16_t retValue { 0 } ;

	try {
		if (!sendCmd (defFlashCmd::cmdD7)) throw 0 ;

		if (!sendCmd (defFlashCmd::cmd0B, defFlashPageSysInfo, defFlashAddrSysSN)) throw 0 ;
		if (!read (mBufRead, 2)) throw 0 ;
		retValue = (mBufRead [0] << 8) | mBufRead [1]  ;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);	// Притягиваем к V PIN управления
	}
	  catch (...) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	  }

	return retValue ;
}
//---------------------------------------------------------
bool TSysInfo::erasePage (const uint16_t inPage)
{
	return sendCmd (defFlashCmd::cmd81, inPage) ;
}
//---------------------------------------------------------
/*!
 * @param inAddr Адрес устройства
 * @param inSn Серийный номер устройства
 * @param inErr Кол-во ошибок
 * @return true если всё хорошо
 */
bool TSysInfo::setSysInfo (const uint8_t inAddr, const uint16_t inSn, const uint32_t inErr)
{
	bool retVal  { false } ;

	try {
		uint8_t bufWrite [12] { 0 } ;		// Буфер для записи данных на флешку

		if (!sendCmd (defFlashCmd::cmdD7)) throw 0 ;
		if (!sendCmd (defFlashCmd::cmd81, defFlashPageSysInfo)) throw 0 ;

		bufWrite [0] = inAddr ;
		bufWrite [2] = inSn >> 8 ;
		bufWrite [3] = inSn & 0xFF ;
		bufWrite [4] = static_cast <uint8_t> (mKey0.size()) ;
		bufWrite [5] = static_cast <uint8_t> (mKey1.size()) ;
		bufWrite [6] = static_cast <uint8_t> (mKey2.size()) ;
		bufWrite [7] = static_cast <uint8_t> (mKey3.size()) ;
		bufWrite [8] = inErr >> 24  ;
		bufWrite [9] = (inErr >> 16) & 0xFF ;
		bufWrite [10] = (inErr >> 8) & 0xFF ;
		bufWrite [11] = inErr & 0xFF ;

		if (!sendCmd (defFlashCmd::cmdD7)) throw 0 ;
		if (!sendCmd (defFlashCmd::cmd85, defFlashPageSysInfo)) throw 0 ;
		if (!transmit (bufWrite, sizeof (bufWrite))) throw 0 ;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);	// Притягиваем к V PIN управления
//		if (!sendCmd (defFlashCmd::cmdD7)) throw 0 ;
	}
	  catch (...) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	  }

	return retVal ;
}
//-----------------------------------------------------------------------
/*!
 * \todo Для проверки корректности записи, хорошо бы посчитать CRC, сравнить и если косяки орать что сдохла флешка
 */
void TSysInfo::writeAlarmData ()
{
	try {
		if ((mAlarmPos + apCount) > defNumByte) {	// Данные не умещайтся на страницу флешки
			if (!erasePage(defFlashPageAlarm)) throw 0 ;
			mAlarmPos = 0 ;
		}
		if (!sendCmd (defFlashCmd::cmdD7)) throw 0 ;
		if (!sendCmd (defFlashCmd::cmd85, defFlashPageSysInfo)) throw 0 ;
		if (!transmit (mAlarmData.data(), mAlarmData.size())) throw 0 ;
		mAlarmPos += apCount ;
	}
	  catch (...) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
		std::shared_ptr <TState> ptrStateUnit (new TState (common::internalFlash, common::defState::flashError)) ;
		app::application -> setQueueTxElement (ptrStateUnit) ;
	  }
}
//-----------------------------------------------------------------------
/*!
 * @param inUnit	Идентификатор обслуживаемого устройства. Если он не попадает в alarmUnit, то софт тупо упадёт
 * @param inPort Бит события который нужно установить.
 * \attention Запись флагов выполнится только если хоть один бит изменился
 */
void TSysInfo::setAlarm (alarmUnit inUnit, uint16_t inPort)
{
	uint8_t alarm = mAlarmData.at(inUnit) & (0x01 << inPort) ;
	if (alarm != mAlarmData.at(inUnit)) {
		mAlarmData [inUnit] = alarm ;
		writeAlarmData () ;
	}
}
//-----------------------------------------------------------------------
/*!
 * @param inUnit ID устройства common::defPort
 * @param inPort Номер порта
 * @return Состояние флага
 */
bool TSysInfo::getAlarm (alarmUnit inUnit, uint16_t inPort)
{
	return (mAlarmData.at(inUnit) & (0x01 << inPort)) ;
}
//-----------------------------------------------------------------------
/*!
 * @param inUnit ID устройства common::defPort
 * @return Информация по всем контролируемым портам (максимум 8)
 */
uint8_t TSysInfo::getAlarm (alarmUnit inUnit)
{
	return mAlarmData.at(inUnit) ;
}
//-----------------------------------------------------------------------
/*!
 * @param inReader Номер считывателя.
 * @param inBuf Ссылка на контейнер для результатов.
 * @return true, если всё хорошо, и false, если что-то пошло не так.
 * \attention Размер контейнера для результатов, должен быть достаточным для чтения всех ключей, т.к. я его не проверяю.
 */
bool TSysInfo::readKeys (const uint8_t inReader, std::vector<uint8_t> & inBuf)
{
	bool retValue { true } ;

	try {
		uint16_t startPage { 0 } ;
		std::vector <uint8_t> tempBuf (defNumByte, 0x0F) ;	// временный буфер для чтения счтаниц с ключами из флешки

		switch (inReader) {
		  case 0x00:
			startPage = defFlashPageKey0 ;
		  break;

		  case 0x01:
			startPage = defFlashPageKey1 ;
		  break;

		  case 0x02:
			startPage = defFlashPageKey2 ;
		  break;

		  case 0x03:
			startPage = defFlashPageKey3 ;
		  break;

		  default:
			throw 0 ; 				// По хорошему тут нужно кидать assert, потому что "усё приплыли"
		  break;
		}

		if (!sendCmd (defFlashCmd::cmdD7)) throw 0 ;
		if (!sendCmd (defFlashCmd::cmd0B, startPage)) throw 0 ;
		if (!read (inBuf)) throw 0 ;
	}
	  catch (...) {
		  retValue = false ;
	  }

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	return retValue ;
}
//-----------------------------------------------------------------------
/*!
 * @param inReader Номер считывателя.
 * @return true, если всё хорошо, и false, если что-то пошло не так.
 * \attention Ключи читается только для одного считывателя. Для всех сразу прочитать не получится
 */
bool TSysInfo::readKeys (const uint8_t inReader)
{
	bool retValue { true } ;
	try {
	    std::unordered_set<common::typeKey> *ptrKey { nullptr } ;

		switch (inReader) {
		  case 0x00:
			ptrKey = &mKey0 ;
		  break;

		  case 0x01:
			ptrKey = &mKey1 ;
		  break;

		  case 0x02:
			ptrKey = &mKey2 ;
		  break;

		  case 0x03:
			ptrKey = &mKey3 ;
		  break;

		  default:
			throw 0 ;
		  break;
		}

		std::vector <uint8_t> bufRead (defNumByte * defKeyNumPage, 0x0F) ;	// Временный буфер в который прочтём все ключи для считывателя
		readKeys(inReader, bufRead) ;
																			//
		for (auto it = bufRead.begin(); it != bufRead.end(); std::advance(it, common::defKeySize)) {
			if (*it == 0xFF) {
				if (*std::next(it, 1) != 0xFF || *std::next(it, 2) != 0xFF || *std::next(it, 3) != 0xFF) {
					common::typeKey key = (*std::next(it, 1) < 16) || (*std::next(it, 2) < 8) || *std::next(it, 3) ; // Нас интересует только 3 байта!!!
					ptrKey -> insert (key) ;
				}
				else break ;		// Все 4 байта равны 0xFF и значит ключей больше нет
			}
		}
	}
	  catch (...) {
		std::shared_ptr <TState> ptrStateUnit (new TState (common::internalFlash, common::defState::flashError)) ;
		app::application -> setQueueTxElement (ptrStateUnit) ;
		retValue = false ;
	  }

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	return retValue ;
}
//-----------------------------------------------------------------------
/*!
 * @param inReader	id Считывателя
 * @param inKey	Ключ
 * @return true если ключ записался
 */
bool TSysInfo::writeKey (const uint8_t inReader, const common::typeKey inKey)
{
	bool retVal { true } ;
	std::vector <uint8_t> bufRead (defNumByte * defKeyNumPage, 0x0F) ;	// Временный буфер в который прочтём все ключи для считывателя
	std::unordered_set<common::typeKey> *ptrKey { nullptr } ;

	switch (inReader) {
	  case 0x00:
		ptrKey = &mKey0 ;
	  break;

	  case 0x01:
		ptrKey = &mKey1 ;
	  break;

	  case 0x02:
		ptrKey = &mKey2 ;
	  break;

	  case 0x03:
		ptrKey = &mKey3 ;
	  break;

	  default:
		throw 0 ;
	  break;
	}

	if (ptrKey -> insert(inKey).second) { 									// ключ не найден
		readKeys(inReader, bufRead) ;
		auto it = bufRead.end () ;											// Проверяем заполненность флешки ключами
		if (*std::prev (it, 4) != 0xFF || *std::prev (it, 3) != 0xFF || *std::prev (it, 2) != 0xFF || *std::prev (it, 4) != 0xFF) {
			shrinkKey (inReader) ;
			readKeys(inReader, bufRead) ;
		}
		it = bufRead.begin() ;
		for (; it != bufRead.end(); std::advance(it, common::defKeySize)) {
			if (*it == 0xFF && *std::next(it, 1) == 0xFF && *std::next(it, 2) == 0xFF && *std::next(it, 3) == 0xFF) {
				auto position = std::distance(bufRead.begin(), it) ;		// вычисляем позицию в которую можно записать ключ
				uint16_t page = position / defNumByte ;
				uint16_t address = position / defNumByte ;
				if (!sendCmd (defFlashCmd::cmdD7)) throw 0 ;
				if (!sendCmd (defFlashCmd::cmd85, page, address + 1)) throw 0 ;

				uint32_t key = inKey << 8 ;									// Ключ у нас 3 байта и поэтому выкидываем не нужный старший байт
				if (!transmit ((uint8_t*) (&key), 3)) throw 0 ;
			}
		}

		retVal = true ;
	}
	  else retVal = false ;

	return retVal ;
}
//-----------------------------------------------------------------------
/*!
 * @param inReader	id Считывателя
 * @param inKey	Ключ
 * @return true если ключ стёрся
 */
bool TSysInfo::deleteKey (uint8_t inReader, common::typeKey inKey)
{
	bool retVal { true } ;

	return retVal ;
}
//-----------------------------------------------------------------------
/*!
 * @param inReader	id Считывателя
 * @return true если ключи стёрлись
 */
bool TSysInfo::deleteKey (uint8_t inReader)
{
	bool retValue { true } ;

	try {
		uint16_t startPage { 0 } ;

		switch (inReader) {
		  case 0x00:
			startPage = defFlashPageKey0 ;
		  break;

		  case 0x01:
			startPage = defFlashPageKey1 ;
		  break;

		  case 0x02:
			startPage = defFlashPageKey2 ;
		  break;

		  case 0x03:
			startPage = defFlashPageKey3 ;
		  break;

		  default:
			throw 0 ; 				// По хорошему тут нужно кидать assert, потому что "усё приплыли"
		  break;
		}

		for (uint32_t id = 0; id < defKeyNumPage; ++id) {
			if (!erasePage(startPage + id)) throw 0  ;
		}
	}
	  catch (...) {
			std::shared_ptr <TState> ptrStateUnit (new TState (common::internalFlash, common::defState::flashError)) ;
			app::application -> setQueueTxElement (ptrStateUnit) ;
			retValue = false ;
	  }

	return retValue ;
}
//-----------------------------------------------------------------------
bool TSysInfo::shrinkKey (const uint8_t inReader)
{
	bool retValue { true } ;
	std::vector <uint8_t> bufRead (defNumByte * defKeyNumPage, 0x0F) ;	// Временный буфер в который прочтём все ключи для считывателя

	try {
		if (!readKeys(inReader, bufRead)) throw 0 ;
		if (!deleteKey (inReader)) throw 0 ;
		for (auto it = bufRead.begin(); it != bufRead.end(); std::advance(it, common::defKeySize)) {
			if (*it == 0x00) continue ;									// Ключ помечен как удалённый
			if (*it == 0xFF && *std::next(it, 1) == 0xFF && *std::next(it, 2) == 0xFF && *std::next(it, 3) == 0xFF) break ; // Усё. Дошли до конца списка
			if (*it == 0xFF && (*std::next(it, 1) !=0xFF || *std::next(it, 2) != 0xFF || *std::next(it, 3) != 0xFF)) { // Нашли ключ

			}

		}
	}
	   catch (...) {
		   retValue = false ;
	   }

	return retValue ;
}
//-----------------------------------------------------------------------
} /* namespace app */
