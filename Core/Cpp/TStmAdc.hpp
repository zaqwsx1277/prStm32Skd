/*
 * TStmAdc.hpp
 *
 *  Created on: Feb 17, 2021
 *      Author: energia
 */

#ifndef CPP_TSTMADC_HPP_
#define CPP_TSTMADC_HPP_

#include <stdint.h>
#include <vector>

#include <stm32f4xx_hal.h>

#include "adc.h"

namespace stm {

//----------------------------------------------------------------
/*!
 * \brief Класс для обработки данных с АЦП
 */
class TStmAdc {
private:
	ADC_HandleTypeDef* mPtrAdcHndl { nullptr } ;
public:
	TStmAdc(ADC_HandleTypeDef*);
	virtual ~TStmAdc();

	bool startConversationDMA (uint32_t *, uint32_t) ;		///< Запуск преобразования с получением данных через DMA
	bool startConversationDMA (std::vector<uint16_t>&) ;	///< Запуск преобразования с получением данных через DMA
};
//----------------------------------------------------------------
} /* namespace stm */

#endif /* CPP_TSTMADC_HPP_ */
