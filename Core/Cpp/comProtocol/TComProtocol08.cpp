/*
 * TComProtocol08.cpp
 *
 *  Created on: Mar 26, 2021
 *      Author: energia
 */

#include <TComProtocol08.hpp>

#include "TApplication.hpp"

namespace comProtocol {

//--------------------------------------------------------------------
/*!
 * @param inBuf	Ссылка на контейнер с полученными данными
 * @param outBuf Ссылка на контейнер, который будет отправлен с ответом
 */
void TComProtocol_08::process (const std::vector<uint8_t> &inBuf, std::vector<uint8_t> &outBuf, const typeErr)
{
									// Даём команду разблокировать нужные замки
	if (inBuf.at(app::defDataPos) == 0 || inBuf.at(app::defDataPos) == 1 ) app::application -> mListUnits [common::defPort::portLc0] -> setProcess(2) ;
	if (inBuf.at(app::defDataPos) == 0 || inBuf.at(app::defDataPos) == 2 ) app::application -> mListUnits [common::defPort::portLc1] -> setProcess(2) ;
	if (inBuf.at(app::defDataPos) == 0 || inBuf.at(app::defDataPos) == 3 ) app::application -> mListUnits [common::defPort::portLc2] -> setProcess(2) ;
	if (inBuf.at(app::defDataPos) == 0 || inBuf.at(app::defDataPos) == 4 ) app::application -> mListUnits [common::defPort::portLc3] -> setProcess(2) ;

	outBuf.push_back (static_cast <uint8_t> (defCommand::cmd08)) ;
	outBuf.push_back (inBuf.at(app::defDataPos)) ;
}
//--------------------------------------------------------------------
} /* namespace comProtocol */
