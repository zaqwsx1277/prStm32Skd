/*
 * TComProtocol13.cpp
 *
 *  Created on: Mar 26, 2021
 *      Author: energia
 */

#include <TComProtocol13.hpp>
#include "TApplication.hpp"

namespace comProtocol {

//--------------------------------------------------------------------
/*!
 * @param inBuf	Ссылка на контейнер с полученными данными
 * @param outBuf Ссылка на контейнер, который будет отправлен с ответом
 */
void TComProtocol_13::process (const std::vector<uint8_t> &inBuf, std::vector<uint8_t> &outBuf, const typeErr)
{
									// Даём команду открыть нужные замки
	app::application -> mListUnits [common::defPort::portLc0] -> setProcess(1) ;
	app::application -> mListUnits [common::defPort::portLc1] -> setProcess(1) ;
	app::application -> mListUnits [common::defPort::portLc2] -> setProcess(1) ;
	app::application -> mListUnits [common::defPort::portLc3] -> setProcess(1) ;

	outBuf.push_back (static_cast <uint8_t> (defCommand::cmd13)) ;
}
//--------------------------------------------------------------------
} /* namespace comProtocol */
