/*
 * TStmGpioOutput.cpp
 *
 *  Created on: Feb 25, 2021
 *      Author: energia
 */

#include <TStmGpioOutput.hpp>

namespace stm {

TStmGpioOutput::TStmGpioOutput (GPIO_TypeDef* inPort, uint16_t inPin) : TStmGpio (inPort, inPin)
{


}
//-------------------------------------------------------------
/*!
 * \brief Установка состояния
 * @param inState Устанавливаемое состояние (on/off/blink)
 * @param inNum Продолжительность установленного состояния в мСек для on/off. Для blink - кол-во изменений. При значении 0 - непрерывное.
 * \todo Вообще не сделана обработка параметра inNum
 */
void TStmGpioOutput::set (gpioState inState, uint32_t inNum)
{
	switch (inState) {
	  case gpOn:
		HAL_GPIO_WritePin(mGpioPort, mGpioPin, GPIO_PIN_SET) ;
	  break;

	  case gpOff:
		HAL_GPIO_WritePin(mGpioPort, mGpioPin, GPIO_PIN_RESET) ;
	  break;

	  default:
	  break;
	}


}
//-------------------------------------------------------------
} /* namespace stm */
