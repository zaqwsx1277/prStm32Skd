/*
 * TWiegand.cpp
 *
 *  Created on: 2 февр. 2021 г.
 *      Author: energia
 */

#include <algorithm>

#include "TWiegand.hpp"

namespace wiegand {
//---------------------------------------------------------
TWiegand::TWiegand() {


}
//---------------------------------------------------------
TWiegand::~TWiegand() {
	// TODO Auto-generated destructor stub
}
//---------------------------------------------------------
sensors::sensorState TWiegand::reset ()
{
	mState = sensors::ssOK ;
	mProtocolType = wiegandUnknown ;
	mPosCount = 0 ;
	mValue = 0 ;
}
//---------------------------------------------------------
/*!
 * \brief Получение значение ключа прочитанного со считывателя
 * \attention Со считывателя приходят данные в десятичном формате и их нужно конвертировать в шестнадцатиричный
 * @return Прочитанный ключ в шестнадцатиричном формате
 */

uint64_t TWiegand::getValue ()
{
	uint64_t retValue { 0 }, tempValue { mValue } ;

	switch (getProtocol ()) {
	  case wiegand26:
		for (uint8_t i = 0; i < 6; ++i) {	// todo нужно переделать для других протоколов
			uint8_t test = (tempValue & 0x0F) ;
			if (test > 9) break ;			// Проверяем корректность переданной цифры. Она должна быть от 0 до 9
			retValue += test * (i == 0 ? 1 : (i + 1) * 10) ;
		}
		mState = sensors::ssReady ;
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
protocolType TWiegand::getProtocol ()
{
	protocolType retValue = wiegandUnknown ;

	switch (mPosCount) {
	  case wiegand26:
	  case wiegand33:
	  case wiegand34:
	  case wiegand35:
	  case wiegand37:
	  case wiegand40:
	  case wiegand42:
		if (chechParity () == true) {
			mValue			retValue = mPosCount ;
		}
	  break;

	  default:
	  break;
	}

	return retValue ;
}
//---------------------------------------------------------
/*!
 * \brief Проверка паритета с учётом типа протокола
 * \todo Пока очень тупо реализован только протокол wiegand26. Это все можно реализовать чисто математически.
 * @return true и успешной проверке.
 */
bool TWiegand::chechParity ()
{
	bool retValue { false } ;
	uint64_t tempValue { mValue } ;

	bool mParity { true }, lParity { false } ;		// Побсчитанное кол-во бит бля старшей и младшей половин

	if (mPosCount == wiegand26) {
	    for (uint8_t i = 0; i < 12; ++i) {
	    	tempValue = tempValue >> 1 ;
	        if ((tempValue & 0x01 ) == 1) lParity++ ; // т.к. lParity всего один бит, то в теории значение должно быть только 0 или 1
	    }
	    for (uint8_t i = 0; i < 12; ++i) {
	    	tempValue = tempValue >> 1 ;
	        if ((tempValue & 0x01 ) == 1) mParity++ ;
	    }
	    if (lParity == (bool) (mValue & 0x01) && mParity == (bool) )
		retValue = true ;
	}
}
//---------------------------------------------------------
void TWiegand::get0 () ;						///< Добавление очередного бита 0 в mValue
void TWiegand::get1 () ;						///< Добавление очередного бита 1 в mValue
} /* namespace protocol */
