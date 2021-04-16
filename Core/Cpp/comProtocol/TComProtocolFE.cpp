/*
 * TComProtocolFE.cpp
 *
 *  Created on: 9 мар. 2021 г.
 *      Author: energia
 */

#include "TComProtocolFE.hpp"
#include "TCommon.hpp"
#include "TApplication.hpp"

namespace comProtocol {

//--------------------------------------------------------------------
/*!
 * @param inBuf	Ссылка на контейнер с полученными данными
 * @param outBuf Ссылка на контейнер, который будет отправлен с ответом
 */
void TComProtocol_FE::process (const std::vector<uint8_t> &inBuf, std::vector<uint8_t> &outBuf,  const typeErr inTypeErr)
{
	outBuf.push_back (static_cast <uint8_t> (defCommand::cmdFE)) ;
	outBuf [app::defSrcPos] = app::application -> mSysInfo.getAddr() ;
	uint16_t serialNumber = app::application -> mSysInfo.getSN() ;
	outBuf.push_back (serialNumber >> 8) ;
	outBuf.push_back (serialNumber & 0xFF) ;
}
//--------------------------------------------------------------------
} /* namespace comProtocol */
