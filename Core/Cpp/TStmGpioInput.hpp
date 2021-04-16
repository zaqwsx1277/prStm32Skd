/*
 * TStmGpioInput.hpp
 *
 *  Created on: Feb 12, 2021
 *      Author: energia
 */

#ifndef CPP_TSTMGPIOINPUT_HPP_
#define CPP_TSTMGPIOINPUT_HPP_

#include <TStmGpio.h>

namespace stm {

class TStmGpioInput: public TStmGpio {
private:
	uint8_t mCountTick { 0 } ;		///< Счётчик тиков для устранения дребезга контактов
	uint16_t mCheckBounce { 0 } ;	///< Значение которое для устранеия контактов должно быть равно 0xFFFF или 0x0000
public:
	TStmGpioInput(GPIO_TypeDef*, uint16_t) ;
	virtual ~TStmGpioInput() = default ;

	bool readData () ;
	bool checkBounce () ;			///< Устранение дребезга контактов
	GPIO_PinState get () ;
};

} /* namespace stm */

#endif /* CPP_TSTMGPIOINPUT_HPP_ */
