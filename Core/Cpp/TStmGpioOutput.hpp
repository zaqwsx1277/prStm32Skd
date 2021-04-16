/*
 * TStmGpioOutput.hpp
 *
 *  Created on: Feb 25, 2021
 *      Author: energia
 */

#ifndef CPP_TSTMGPIOOUTPUT_HPP_
#define CPP_TSTMGPIOOUTPUT_HPP_

#include "TStmGpio.h"

namespace stm {

class TStmGpioOutput : public TStmGpio {
public:
	TStmGpioOutput (GPIO_TypeDef*, uint16_t);
	virtual ~TStmGpioOutput() = default;

	void set (const gpioState inState, const uint32_t inNum = 0) ;			///< Установка состояния
};

} /* namespace stm */

#endif /* CPP_TSTMGPIOOUTPUT_HPP_ */
