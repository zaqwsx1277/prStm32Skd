/*
 * TComProtocol00.cpp
 *
 *  Created on: Mar 17, 2021
 *      Author: AAL
 */

#include <TComProtocol00.hpp>
#include "TApplication.hpp"

namespace comProtocol {

//--------------------------------------------------------------------
/*!
 * @param inBuf	Ссылка на контейнер с полученными данными
 * @param outBuf Ссылка на контейнер, который будет отправлен с ответом
 */
void TComProtocol_00::process (const std::vector<uint8_t> &inBuf, std::vector<uint8_t> &outBuf, const typeErr)
{
	auto address = app::application -> mSysInfo.getAddr () ;
	uint16_t serialNumber = inBuf [app::defDataPos] << 8;
	serialNumber = serialNumber	| inBuf [app::defDataPos + 1] ;

	auto numErr = app::application -> mSysInfo.getNumErr() ;
	app::application -> mSysInfo.setSysInfo(address, serialNumber, numErr) ;

	outBuf[app::defSrcPos] = app::application -> mSysInfo.getAddr () ;
	outBuf.push_back (static_cast <uint8_t> (defCommand::cmd00)) ;
	serialNumber = app::application -> mSysInfo.getSN () ;
	outBuf.push_back (serialNumber >> 8) ;
	outBuf.push_back (serialNumber & 0xFF) ;
}

} /* namespace comProtocol */
