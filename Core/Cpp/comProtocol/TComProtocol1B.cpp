/*
 * TComProtocol1B.cpp
 *
 *  Created on: 18 мар. 2021 г.
 *      Author: AAL
 */

#include "rtc.h"

#include <TComProtocol1B.hpp>
#include "TProtocol.hpp"

namespace comProtocol {

//-----------------------------------------------------------
/*!
 * @param inBuf Контейнер с входящими данными
 * @param outBuf Контейнер с ответом.
 */
void TComProtocol_1B::process (const std::vector<uint8_t> &inBuf, std::vector<uint8_t> &outBuf, const typeErr inTypeErr)
{
	outBuf.push_back (static_cast <uint8_t> (defCommand::cmd1B)) ;
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, inBuf.at (app::defSrcPos));	// В backup регистр записываем адрес блока на который нужно послать команду об успешном перезапуске контроллера
	NVIC_SystemReset () ;
	outBuf.push_back (0) ;		// Если мы попали сюда, то перезапуск не выполнен и поэтому отправляем на концентратор команду об ошибке
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0);
}
//-----------------------------------------------------------
} /* namespace comProtocol */
