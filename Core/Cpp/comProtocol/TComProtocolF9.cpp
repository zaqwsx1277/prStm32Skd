/*
 * TComProtocolF9.cpp
 *
 *  Created on: Mar 25, 2021
 *      Author: energia
 */

#include <TComProtocolF9.hpp>

namespace comProtocol {

//-----------------------------------------------------------
/*!
 * @param inBuf	Ссылка на контейнер с полученными данными
 * @param outBuf Ссылка на контейнер с ответом.
 * \todo Переделать на нормальные классы GPIO
 * \attention При всякой лабуде в поле Сom выполняется запрос данных (аналог 1)
 */
void TComProtocol_F9::process (const std::vector<uint8_t> &inBuf, std::vector<uint8_t> &outBuf, const typeErr)
{
	if (inBuf.at (app::defDataPos) == 1) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, inBuf.at (app::defDataPos + 1) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET) ;
	}
	outBuf.push_back (static_cast <uint8_t> (defCommand::cmdF9)) ;
	outBuf.push_back (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET ? 0 : 1) ;
}
//-----------------------------------------------------------
} /* namespace comProtocol */
