/*
 * TComProtocolF0.cpp
 *
 *  Created on: 5 мар. 2021 г.
 *      Author: energia
 */

#include <TComProtocolF0.hpp>

namespace comProtocol {

//--------------------------------------------------------------------
/*!
 * @param inBuf	Ссылка на контейнер с полученными данными
 * @param outBuf Ссылка на контейнер, который будет отправлен с ответом
 * @param inTypeErr Тип обрабатываемой ошибки
 */
void TComProtocol_F0::process (const std::vector<uint8_t> &inBuf, std::vector<uint8_t> &outBuf,  const typeErr inTypeErr)
{
	outBuf.push_back (static_cast <uint8_t> (defCommand::cmdF0)) ;
	outBuf.push_back (inBuf.at (app::defTypePos)) ;
	outBuf.push_back (static_cast <uint8_t> (inTypeErr)) ;
}
//--------------------------------------------------------------------
} /* namespace stm */
