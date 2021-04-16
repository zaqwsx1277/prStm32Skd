/*
 * TComProtocolFD.cpp
 *
 *  Created on: 16 мар. 2021 г.
 *      Author: energia
 */

#include "rtc.h"

#include <TComProtocolFD.hpp>

namespace comProtocol {

//-----------------------------------------------------------
/*!
 * @param inBuf Контейнер с входящими данными
 * @param outBuf Контейнер с ответом. Если команда не выполнена, то в DATA заполняется 0xFF
 * \todo Переделать работу с RTC
 */
void TComProtocol_FD::process (const std::vector<uint8_t> &inBuf, std::vector<uint8_t> &outBuf, const typeErr inTypeErr)
{
	RTC_TimeTypeDef time {0} ;
	RTC_DateTypeDef date = {0} ;

	outBuf.push_back (static_cast <uint8_t> (defCommand::cmdFD)) ;

	date.Year = inBuf.at (app::defDataPos) ;
	date.Month = inBuf.at (app::defDataPos + 1) ;
	date.Date = inBuf.at (app::defDataPos + 2) ;
	time.Hours = inBuf.at (app::defDataPos + 3) ;
	time.Minutes = inBuf.at (app::defDataPos + 4) ;
	time.Seconds = inBuf.at (app::defDataPos + 5) ;

	try {
		if (HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN) != HAL_OK) throw 0 ;
		if (HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN) != HAL_OK) throw 0 ;
		if (HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN) != HAL_OK) throw 0 ;
		if (HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN) != HAL_OK) throw 0 ;
		auto startPos = inBuf.begin() + app::defDataPos ;
		outBuf.insert (outBuf.end(), startPos, startPos + 6) ;
	}
	  catch (...) {
		  outBuf.insert (outBuf.end(), 6, 0xFF) ;
	  }
}
//-----------------------------------------------------------
} /* namespace comProtocol */
