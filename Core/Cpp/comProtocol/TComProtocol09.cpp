/*
 * TComProtocol09.cpp
 *
 *  Created on: 9 апр. 2021 г.
 *      Author: energia
 */

#include <TComProtocol09.hpp>
#include "TApplication.hpp"
#include "TCommon.hpp"

namespace comProtocol {

//--------------------------------------------------------------------
/*!
 * @param inBuf	Ссылка на контейнер с полученными данными
 * @param outBuf Ссылка на контейнер, который будет отправлен с ответом
 */
void TComProtocol_09::process (const std::vector<uint8_t> &inBuf, std::vector<uint8_t> &outBuf, const typeErr)
{
	uint8_t numKey { 0 } ;		// Кол-во записанных ключей

	uint8_t reader = inBuf.at(app::defDataPos) ;
	common::typeKey key = inBuf.at(app::defDataPos + 1) << 8 ;
	key = (key | inBuf.at(app::defDataPos + 2)) << 8 ;
	key = key | inBuf.at(app::defDataPos + 2) ;

	if (reader == 0 || reader == 0x01) {
		if (app::application -> mSysInfo.writeKey(0, key)) ++numKey ;
	}
	if (reader == 0 || reader == 0x02) {
		if (app::application -> mSysInfo.writeKey(1, key)) ++numKey ;
	}
	if (reader == 0 || reader == 0x03) {
		if (app::application -> mSysInfo.writeKey(2, key)) ++numKey ;
	}
	if (reader == 0 || reader == 0x04) {
		if (app::application -> mSysInfo.writeKey(3, key)) ++numKey ;
	}

	outBuf.push_back (static_cast <uint8_t> (defCommand::cmd09)) ;
	outBuf.push_back (inBuf.at(app::defDataPos)) ;
	outBuf.push_back (numKey) ;
}
//--------------------------------------------------------------------

} /* namespace comProtocol */
