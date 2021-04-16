/*
 * TComProtocolA2.cpp
 *
 *  Created on: 18 мар. 2021 г.
 *      Author: AAL
 */

#include <TComProtocolA2.hpp>
#include "TCommon.hpp"
#include "TApplication.hpp"
#include "TUnitTemperature.hpp"

using namespace app;

namespace comProtocol {

//-----------------------------------------------------------
/*!
 * @param outBuf Ссылка на контейнер с ответом.
 */
void TComProtocol_A2::process (const std::vector<uint8_t> &, std::vector<uint8_t> &outBuf, const typeErr inTypeErr)
{
	RTC_DateTypeDef date {0};
	RTC_TimeTypeDef time {0};

	outBuf.push_back (static_cast <uint8_t> (defCommand::cmdA2)) ;
	outBuf.push_back (common::defVersion) ;
	outBuf.push_back (common::defTypeSKD) ;
	auto temp = static_cast <app::TUnitTemperature *> (app::application -> mListUnits [common::defPort::portTemp]) -> getTemperature() ;
	outBuf.push_back (temp >> 8) ;
	outBuf.push_back (temp & 0xFF) ;
	auto numErr = app::application -> mSysInfo.getNumErr() ;
	outBuf.push_back (static_cast <uint8_t> (numErr >> 24)) ;
	outBuf.push_back (static_cast <uint8_t> ((numErr >> 16) & 0xFF)) ;
	outBuf.push_back (static_cast <uint8_t> ((numErr >> 8) & 0xFF)) ;
	outBuf.push_back (static_cast <uint8_t> (numErr & 0xFF)) ;
	outBuf.push_back (app::application -> mSysInfo.getNumFlashErr()) ;
	outBuf.push_back (app::application -> getQueueTxSize()) ;
	outBuf.push_back (static_cast <uint8_t> (app::application -> mSysInfo.getNumKey0 () >> 8)) ;
	outBuf.push_back (static_cast <uint8_t> (app::application -> mSysInfo.getNumKey0 () & 0xFF)) ;
	outBuf.push_back (static_cast <uint8_t> (app::application -> mSysInfo.getNumKey1 () >> 8)) ;
	outBuf.push_back (static_cast <uint8_t> (app::application -> mSysInfo.getNumKey1 () & 0xFF)) ;
	outBuf.push_back (static_cast <uint8_t> (app::application -> mSysInfo.getNumKey2 () >> 8)) ;
	outBuf.push_back (static_cast <uint8_t> (app::application -> mSysInfo.getNumKey2 () & 0xFF)) ;
	outBuf.push_back (static_cast <uint8_t> (app::application -> mSysInfo.getNumKey3 () >> 8)) ;
	outBuf.push_back (static_cast <uint8_t> (app::application -> mSysInfo.getNumKey3 () & 0xFF)) ;
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN) ;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN) ;
	outBuf.push_back (date.Year) ;
	outBuf.push_back (date.Month) ;
	outBuf.push_back (date.Date) ;
	outBuf.push_back (time.Hours) ;
	outBuf.push_back (time.Minutes) ;
	outBuf.push_back (time.Seconds) ;
}
//-----------------------------------------------------------
} /* namespace comProtocol */
