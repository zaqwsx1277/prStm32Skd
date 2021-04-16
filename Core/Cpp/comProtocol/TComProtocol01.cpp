/*
 * TComProtocol01.cpp
 *
 *  Created on: 10 мар. 2021 г.
 *      Author: AAL
 */

#include "TComProtocol01.hpp"
#include "TProtocol.hpp"
#include "TCommon.hpp"
#include "TApplication.hpp"

namespace comProtocol {

//--------------------------------------------------------------------
/*!
 * @param inBuf	Ссылка на контейнер с полученными данными
 * @param outBuf Ссылка на контейнер, который будет отправлен с ответом
 * @param inTypeErr На хрен тут не нужен
 * \todo Сделать обработку ошибки записи на флеш
 */
void TComProtocol_01::process (const std::vector<uint8_t> &inBuf, std::vector<uint8_t> &outBuf, const typeErr)
{
	uint16_t serialNumber = inBuf [app::defCom01SnPos] << 8;
	serialNumber = serialNumber	| inBuf [app::defCom01SnPos + 1] ;

	auto numErr = app::application -> mSysInfo.getNumErr() ;
	app::application -> mSysInfo.setSysInfo(inBuf [app::defCom01AdrPos], serialNumber, numErr) ;

	outBuf[app::defSrcPos] = app::application -> mSysInfo.getAddr () ;
	outBuf.push_back (static_cast <uint8_t> (defCommand::cmd01)) ;
	serialNumber = app::application -> mSysInfo.getSN () ;
	outBuf.push_back (serialNumber >> 8) ;
	outBuf.push_back (serialNumber & 0xFF) ;
}
//--------------------------------------------------------------------
} /* namespace comProtocol */
