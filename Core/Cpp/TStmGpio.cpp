/*
 * TStmGpio.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: energia
 */

#include <TStmGpio.h>

namespace stm {
//------------------------------------------------------------
/*!
 * \brief Конструктор
 * @param inGpioPort
 * @param inGpioPin
 */
TStmGpio::TStmGpio(GPIO_TypeDef* inGpioPort, uint16_t inGpioPin)
{
	mGpioPort = inGpioPort ;
	mGpioPin = inGpioPin ;
}
//---------------------------------------------------------------

} /* namespace stm */
