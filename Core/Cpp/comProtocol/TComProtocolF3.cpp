/*
 * TComProtocolF3.cpp
 *
 *  Created on: Apr 1, 2021
 *      Author: AAL
 */

#include <TComProtocolF3.hpp>
#include "TApplication.hpp"

namespace comProtocol {

//-----------------------------------------------------------
/*!
 * @param inBuf Ссылка на контейнер с принятой командой.
 * @param outBuf Ссылка на контейнер с ответом.
 * \attention При удалении ключей из всех считывателей может произойти неоднозначная ситуация, когда ключи будут удалены не из всех считывателей.
 */
void TComProtocol_F3::process (const std::vector<uint8_t> &inBuf, std::vector<uint8_t> &outBuf, const typeErr inTypeErr)
{
	bool isDelete { true } ;

	outBuf.push_back (static_cast <uint8_t> (defCommand::cmdF3)) ;
	outBuf.push_back (inBuf[app::defDataPos]) ;

	if (inBuf[app::defDataPos] == 0x00 || inBuf[app::defDataPos] == 0x01) isDelete = app::application -> mSysInfo.deleteKey(0x00) ;
	if (isDelete && (inBuf[app::defDataPos] == 0x00 || inBuf[app::defDataPos] == 0x02)) isDelete = app::application -> mSysInfo.deleteKey(0x01) ;
	if (isDelete && (inBuf[app::defDataPos] == 0x00 || inBuf[app::defDataPos] == 0x03)) isDelete = app::application -> mSysInfo.deleteKey(0x02) ;
	if (isDelete && (inBuf[app::defDataPos] == 0x00 || inBuf[app::defDataPos] == 0x04)) isDelete = app::application -> mSysInfo.deleteKey(0x03) ;
	if (isDelete) outBuf.push_back (1) ;
	  else outBuf.push_back (2) ;
}
//-----------------------------------------------------------
} /* namespace comProtocol */
