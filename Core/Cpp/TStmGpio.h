/*
 * TStmGpio.h
 *
 *  Created on: Feb 12, 2021
 *      Author: AAL
 */

#ifndef CPP_TSTMGPIO_H_
#define CPP_TSTMGPIO_H_

#include "stm32f4xx_hal.h"

namespace stm {

/// Возможные состояния GPIO
enum gpioState {
	gpUnknown,		///< Х.з. что такое
	gpOk,			///< Всё очень хорошо
	gpErr,			///< А это уже полный пиздец
	gpOn,			///< Вкл.
	gpOff,			///< Выкл.
	gpBlink			///< Мигает
};

/*!
 * \brief Класс описания портов GPIO
 */
class TStmGpio {
protected:
	gpioState mState { gpOk } ;

	GPIO_TypeDef* mGpioPort { nullptr } ;
	uint16_t mGpioPin { 0 } ;
public:
	TStmGpio (GPIO_TypeDef*, uint16_t);
	virtual ~TStmGpio() = default ;

};

} /* namespace stm */

#endif /* CPP_TSTMGPIO_H_ */
