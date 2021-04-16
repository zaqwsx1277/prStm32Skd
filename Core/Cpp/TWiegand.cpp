/*
 * TWiegand.cpp
 *
 *  Created on: 2 февр. 2021 г.
 *      Author: energia
 */

#include <algorithm>

#include "TWiegand.hpp"

namespace reader {
/*!
 * \brief Конструктор
 */
//---------------------------------------------------------
TWiegand::TWiegand()
{
	reset () ;
}
//---------------------------------------------------------
sensors::sensorState TWiegand::reset ()
{
	sensors::sensorState retValue  { sensors::ssOK } ;

	mState = sensors::ssOK ;
	mProtocolType = wiegandUnknown ;
	mPosCount = 0 ;
	mValue = 0 ;

	return retValue ;
}
//---------------------------------------------------------
/*!
 * \brief Получение значение ключа прочитанного со считывателя
 * @return Прочитанный ключ. При ошибке возвращается 0xFFFFFFFFFFFFFFFF
 */

uint64_t TWiegand::getValue ()
{
	uint64_t retValue { 0xFFFFFFFFFFFFFFFF } ;

	switch (checkProtocol ()) {
	  case wiegand26:
		if (chechParity() == true) {
			retValue = mValue >> 1 ;
			retValue = retValue & 0x1FFFFFF ;
			mState = sensors::ssOK ;
		}
		else mState = sensors::ssErrorHW ;
	  break;

	  default:
		mState = sensors::ssErrorSW ;
	  break;
	}
	return retValue ;

}
//---------------------------------------------------------
/*!
 * \brief Проверка типа протокола.
 * \details Если при проверке выявляются какие-либо ошибки, то возвращается тип wiegandUnknown
 * @return Тип обрабатываемого протокола
 */
protocolType TWiegand::checkProtocol ()
{
	protocolType retValue = wiegandUnknown ;

	switch (mPosCount) {
	  case wiegand26:
		  retValue = wiegand26 ;
	  break ;
//	  case wiegand33:
//	  case wiegand34:
//	  case wiegand35:
//	  case wiegand37:
//	  case wiegand40:
//	  case wiegand42:
//		if (chechParity () == true) {
//			retValue = wiegand26 ;
//		}
//	  break;

	  default:
	  break;
	}

	return retValue ;
}
//---------------------------------------------------------
/*!
 * \brief Проверка паритета с учётом типа протокола
 * \todo Пока очень тупо реализован только протокол wiegand26.
 * @return Флаг проверки. Возвращает true при успешной проверке.
 */
bool TWiegand::chechParity ()
{
	bool retValue { false } ;
	uint64_t tempValue { mValue } ;

	bool mParity { true }, lParity { false } ;		// Побсчитанное кол-во бит бля старшей и младшей половин

	if (mPosCount == wiegand26) {
	    for (uint8_t i = 0; i < 12; ++i) {
	    	tempValue = tempValue >> 1 ;
	        if ((tempValue & 0x01 ) == 1) lParity ^= true ;
	    }
	    for (uint8_t i = 0; i < 12; ++i) {
	    	tempValue = tempValue >> 1 ;
	        if ((tempValue & 0x01 ) == 1) mParity ^= true ;
	    }
	    if (lParity == (bool) (mValue & 0x01) && mParity == (bool) (mValue & 0x2000000))
	    	retValue = true ;
	}
	return retValue ;
}
//---------------------------------------------------------
/*!
 * @param inData данные который прилетели от считывателя
 * \todo Протестировать на тупой поток импульсов, т.к. при какой-то ситуации (х.з. какой) котроллер летит по памяти
 */
void TWiegand::get (uint8_t inData)
{
	switch (mState) {
	  case sensors::ssOK:
	  case sensors::ssWait:
		mValue <<= 1 ;
		mValue |= (0x01 & inData) ;		// добавляю только один бит
		mPosCount ++ ;
		mTimeFinish = HAL_GetTick() ;
		mState = sensors::ssWait ;
	  break;

	  case sensors::ssReady : 	// тупо игнорирую обработку, т.к. предыдущий ключ ещё на обработан
	  break ;

	  default:							// Реакция на непонятное состояние. Нпроимер, ошибку
		mValue = 0 ;
		mPosCount = 0 ;
		mTimeFinish = 0 ;
		mState = sensors::ssOK ;
	  break;
	}

	if (mPosCount == 26)		// !!! Косяк. обрабатываю только 26 бит
		mTimeFinish = HAL_GetTick() ;
}
//---------------------------------------------------------
/*!
 * @brief Проверка периода окончания посылки
 * @return Возврат true если с момента получения последнего импульса прошло defStopPeriod мСек.
 * @details Если время истекло
 */
bool TWiegand::checkFinish ()
{
	bool retValue { false } ;
									// Ловим период более 250 мСек (признак окончания посылки)
	if ((HAL_GetTick() - mTimeFinish) > defStopPeriod && mState == sensors::ssWait) {
		mState = sensors::ssReady ;
		retValue = true ;
	}
									// Если признак окончания посылки не приходит
	if (mPosCount > wiegand42) mState = sensors::ssErrorHW ;

	return retValue ;
}
//---------------------------------------------------------
} /* namespace reader */
