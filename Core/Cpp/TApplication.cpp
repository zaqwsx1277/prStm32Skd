/*
 * X.cpp
 *
 *  Created on: Feb 15, 2021
 *      Author: AAL
 */

#include <algorithm>

#include "tim.h"

#include "TApplication.hpp"
#include "TCommon.hpp"
#include "TUnitOpen.hpp"
#include "TUnitReader.hpp"
#include "TUnitReaderAdc.hpp"
#include "TUnitReaderLed.hpp"
#include "TUnitLock.hpp"
#include "TComProtocol.hpp"

namespace app {

std::unique_ptr <TApplication> application (nullptr) ;	///< Указатель на данные для обеспечения работы приложения

//-----------------------------------------------------------------------------------------------
/*!
 * \todo Нужно приделать обработку возможных ошибок когда create () возвращает nullptr
 */
TApplication::TApplication() {

	if (mSysInfo.checkFlash() != true) {				// Проверяем жизнеспособность флешки
		std::shared_ptr <TState> ptrStateUnit (new TState (common::defPort::internalFlash, common::defState::flashError)) ;
		application -> setQueueTxElement (ptrStateUnit) ;
	}
		// Задаём внутренние номера портов и не забываем что они должны быть уникальные!!!
	mListUnits [common::defPort::portRd0] = TUnit::create(utReader, common::defPort::portRd0) ;
	mListUnits [common::defPort::portRd1] = TUnit::create(utReader, common::defPort::portRd1) ;
	mListUnits [common::defPort::portRd2] = TUnit::create(utReader, common::defPort::portRd2) ;
	mListUnits [common::defPort::portRd3] = TUnit::create(utReader, common::defPort::portRd3) ;
	mListUnits [common::defPort::portRd0] -> setRoomId (0) ;		// todo здесь всё должно быть переделано на программируемую конфигурацию
	mListUnits [common::defPort::portRd1] -> setRoomId (1) ;
	mListUnits [common::defPort::portRd2] -> setRoomId (2) ;
	mListUnits [common::defPort::portRd3] -> setRoomId (3) ;

	mListUnits [common::defPort::portLc0] = TUnit::create(utLock, common::defPort::portLc0) ;
	mListUnits [common::defPort::portLc1] = TUnit::create(utLock, common::defPort::portLc1) ;
	mListUnits [common::defPort::portLc2] = TUnit::create(utLock, common::defPort::portLc2) ;
	mListUnits [common::defPort::portLc3] = TUnit::create(utLock, common::defPort::portLc3) ;
	mListUnits [common::defPort::portLc0] -> setRoomId (0) ;		// todo здесь всё должно быть переделано на программируемую конфигурацию
	mListUnits [common::defPort::portLc1] -> setRoomId (1) ;
	mListUnits [common::defPort::portLc2] -> setRoomId (2) ;
	mListUnits [common::defPort::portLc3] -> setRoomId (3) ;

	mListUnits [common::defPort::portDs0] = TUnit::create(utDoorSensor, common::defPort::portDs0) ;
	mListUnits [common::defPort::portDs1] = TUnit::create(utDoorSensor, common::defPort::portDs1) ;
	mListUnits [common::defPort::portDs2] = TUnit::create(utDoorSensor, common::defPort::portDs2) ;
	mListUnits [common::defPort::portDs3] = TUnit::create(utDoorSensor, common::defPort::portDs3) ;
	mListUnits [common::defPort::portDs0] -> setRoomId (0) ;		// todo здесь всё должно быть переделано на программируемую конфигурацию
	mListUnits [common::defPort::portDs1] -> setRoomId (1) ;
	mListUnits [common::defPort::portDs2] -> setRoomId (2) ;
	mListUnits [common::defPort::portDs3] -> setRoomId (3) ;

	mListUnits [common::defPort::portRdLed0] = TUnit::create(utReaderLed, common::defPort::portRdLed0) ;
	mListUnits [common::defPort::portRdLed1] = TUnit::create(utReaderLed, common::defPort::portRdLed1) ;
	mListUnits [common::defPort::portRdLed2] = TUnit::create(utReaderLed, common::defPort::portRdLed2) ;
	mListUnits [common::defPort::portRdLed3] = TUnit::create(utReaderLed, common::defPort::portRdLed3) ;
	mListUnits [common::defPort::portRdLed0] -> setRoomId (0) ;		// todo здесь всё должно быть переделано на программируемую конфигурацию
	mListUnits [common::defPort::portRdLed1] -> setRoomId (1) ;
	mListUnits [common::defPort::portRdLed2] -> setRoomId (2) ;
	mListUnits [common::defPort::portRdLed3] -> setRoomId (3) ;

	mListUnits [common::defPort::portRdAdc] = TUnit::create(utReaderAdc, common::defPort::portRdAdc) ;

	mListUnits [common::defPort::portOpen] = TUnit::create(utSensorOpen, common::defPort::portOpen) ;

	mListUnits [common::defPort::portTemp] = TUnit::create(utSensorTemperature, common::defPort::portTemp) ;

}
//-----------------------------------------------------------------------------------------------
/*!
 * @brief Обработчик состояний устройств
 */
void TApplication::manager ()
{
	switch (mAppState) {
	  case appStarted: {
		mAppState = appProcess ; 			// Игнорируем любые прерывания пока не уйдём в основной цикл ;
		uint16_t isRestart = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);
		if (isRestart != 0) {				// Формируем буфер для ответа на перезапуск контроллера
			mPtrProtocolRS486 -> receiveByte(defStartByte) ;
			mPtrProtocolRS486 -> receiveByte(isRestart) ;
			mPtrProtocolRS486 -> receiveByte(mSysInfo.getAddr ()) ;
			mPtrProtocolRS486 -> receiveByte(defVersion) ;
			mPtrProtocolRS486 -> receiveByte(static_cast <uint8_t> (comProtocol::defCommand::cmdA1)) ;
			mPtrProtocolRS486 -> receiveByte(0) ;
			mPtrProtocolRS486 -> receiveByte(0) ;
			mPtrProtocolRS486 -> receiveByte(defEndByte) ;
			mPtrProtocolRS486 -> receiveByte(defStopByte) ;
			mPtrProtocolRS486 -> setRxCRC () ;
			HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0);
			mAppState = appReceiveMessage ;
		}
		mPtrProtocolRS486 -> startReceiveByte() ;
	  }
	  break;

	  case appProcess :						// Основной режим обработки
		  // Обрабатываем различные состояния считывателей
			mListUnits [common::defPort::portRd0] = TUnit::create(utReader, common::defPort::portRd0) ;
			mListUnits [common::defPort::portRd1] = TUnit::create(utReader, common::defPort::portRd1) ;
			mListUnits [common::defPort::portRd2] = TUnit::create(utReader, common::defPort::portRd2) ;
			mListUnits [common::defPort::portRd3]

		  // Проверка состояния крышки блока
		app::application -> mListUnits [common::defPort::portOpen] -> process() ;

//		  // Обрабатываем данные с АЦП ридеров и температуры
//		app::application -> mListUnits [common::defPort::portRdAdc] -> process() ;
	  break ;

	  case appReceiveMessage:				// Формируем ответ на полученную по RS485 команду
//		  if (mPtrProtocolRS486 -> makeAnswer() == false)   ;
		  mPtrProtocolRS486 -> makeAnswer() ;
		  mAppState = appProcess ;
	  break ;

	  default:
		  mAppState = appProcess ; 			// х.з состояния не может быть и поэтому запускаем основной режим
	  break;
	}
}

//-----------------------------------------------------------------------------------------------
/*!
 * \brief Устранение дребезга контактов для всех кнопок присутствующих в проекте
 */
void TApplication::checkBounce ()
{
												// Проверяем состояние крышки гроба
	static_cast <app::TUnitOpen *> (app::application -> mListUnits [common::defPort::portOpen]) -> checkBounce() ;
}
//-----------------------------------------------------------------------------------------------
/*!
 * \brief Запуск преобразования всех АЦП
 */
void TApplication::startConversation ()
{
	static_cast <app::TUnitReaderAdc *> (app::application -> mListUnits [common::defPort::portRdAdc]) -> startConversation () ;
}
//-----------------------------------------------------------------------------------------------
void TApplication::setQueueReaderElement (std::shared_ptr <TState> inState)
{
	if (mQueueReader.size() > common::defQueueReaderLength) {
		mQueueReader.pop_back() ;
		std::shared_ptr <TState> ptrStateUnit (new TState (inState ->getUintId(), common::defState::hwError, inState -> getRoomId ())) ;
		setQueueTxElement (ptrStateUnit) ;
	}
	mQueueReader.push_front(inState) ;
}
//-----------------------------------------------------------------------------------------------
void TApplication::setgetQueueOtherElement (std::shared_ptr <TState> inState)
{
	if (mQueueOther.size() > common::defQueueOtherLength) {
		mQueueOther.pop_back() ;
		std::shared_ptr <TState> ptrStateUnit (new TState (inState ->getUintId(), common::defState::hwError, inState -> getRoomId ())) ;
		setQueueTxElement (ptrStateUnit) ;
	}
	mQueueOther.push_front(inState) ;
}
//-----------------------------------------------------------------------------------------------
void TApplication::setQueueTxElement (std::shared_ptr <TState> inState)
{
	if (mQueueTx.size() > common::defQueueTxLength) {
		mQueueTx.pop_back() ;
		mQueueTx.pop_back() ;
		std::shared_ptr <TState> ptrStateUnit (new TState (inState ->getUintId(), common::defState::maxQueueTX, inState -> getRoomId ())) ;
		mQueueTx.push_back (ptrStateUnit) ;
	}
	mQueueTx.push_front (inState) ;
}
//-----------------------------------------------------------------------------------------------
/*!
 * @return Указатель на первый элемент в очереди или nullptr
 */
std::shared_ptr <TState> TApplication::TApplication::getQueueReaderElement ()
{
	std::shared_ptr <TState> retValue { nullptr } ;
	if (mQueueReader.size() != 0) {
		retValue = mQueueReader.back() ;
		mQueueReader.pop_back() ;
	}
	return retValue ;
}
//-----------------------------------------------------------------------------------------------
/*!
 * @return Указатель на первый элемент в очереди или nullptr
 */
std::shared_ptr <TState> TApplication::getQueueOtherElement ()
{
	std::shared_ptr <TState> retValue { nullptr } ;
	if (mQueueOther.size() != 0) {
		retValue = mQueueOther.back() ;
		mQueueOther.pop_back() ;
	}
	return retValue ;
}
//-----------------------------------------------------------------------------------------------
/*!
 * @return Указатель на первый элемент в очереди или nullptr
 */
std::shared_ptr <TState> TApplication::getQueueTxElement ()
{
	std::shared_ptr <TState> retValue { nullptr } ;
	auto it = std::find_if (mQueueTx.begin (), mQueueTx.end (), [] (std::shared_ptr <TState> &inState) { return (inState -> getTxState () == common::stTxReady) ;} )  ;
	if (it != mQueueTx.end ()) retValue = *it ;
	  else retValue = mQueueTx.front() ;
	return retValue ;
}
//-----------------------------------------------------------------------------------------------
}
