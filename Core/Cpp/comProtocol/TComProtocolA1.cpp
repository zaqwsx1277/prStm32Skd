/*
 * TComProtocolA1.cpp
 *
 *  Created on: 18 мар. 2021 г.
 *      Author: energia
 */

#include <TComProtocolA1.hpp>

namespace comProtocol {

//-----------------------------------------------------------
/*!
 * @param inBuf Контейнер с входящими данными
 * @param outBuf Контейнер с ответом.
 * \attention
 */
void TComProtocol_A1::process (const std::vector<uint8_t> &inBuf, std::vector<uint8_t> &outBuf, const typeErr inTypeErr)
{
	outBuf.push_back (static_cast <uint8_t> (defCommand::cmd1B)) ;
	outBuf.push_back (1) ;
}

} /* namespace comProtocol */
