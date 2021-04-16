/*
 * TUnitReaderAdc.hpp
 *
 *  Created on: Feb 17, 2021
 *      Author: AAL
 */

#ifndef CPP_TUNITREADERADC_HPP_
#define CPP_TUNITREADERADC_HPP_

#include <memory>
#include <vector>

#include "TStmAdc.hpp"
#include "TUnit.hpp"
#include "TCommon.hpp"

namespace app {

/*!
 * \brief Класс для обработки данных с АЦП ридеров + получение температуры процессора
 * \attention Т.к. стек отправлен в CCMRAM, то mBufData сделан static, т.к. получение данных сделано через DMA
 */
class TUnitReaderAdc : public TUnit, stm::TStmAdc {
private:
	std::vector <uint16_t> mBufData = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF } ;
public:

	TUnitReaderAdc(ADC_HandleTypeDef*) ;
	virtual ~TUnitReaderAdc() = default ;

	unitState process () override ;			///< Обработка состояния АЦП и датчика температуры
	void setProcess (uint32_t inData = 0) { mProcessData = inData ; } 	///< Установка флага выполнения

	void startConversation () ;				///< Запуск АЦП

};

} /* namespace app */

#endif /* CPP_TUNITREADERADC_HPP_ */
