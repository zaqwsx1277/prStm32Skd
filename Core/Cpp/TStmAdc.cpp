/*
 * TStmAdc.cpp
 *
 *  Created on: Feb 17, 2021
 *      Author: energia
 */

#include <TStmAdc.hpp>
#include "TCommon.hpp"

namespace stm {
//--------------------------------------------------------
TStmAdc::TStmAdc(ADC_HandleTypeDef* inHndl) {
	mPtrAdcHndl = inHndl ;
}
//--------------------------------------------------------
TStmAdc::~TStmAdc() {
	HAL_ADC_Stop_DMA(mPtrAdcHndl);
}
//--------------------------------------------------------
/*!
 * \brief Запуск преобразования с получением данных через DMA
 * @param inBuf Буфер куда будут записаны считанные данные
 * @param inSize Размер буфера
 * @return true при успешном запуске
 */
bool TStmAdc::startConversationDMA (uint32_t *inBuf, uint32_t inSize)
{
	bool retValue { false } ;
	if (mPtrAdcHndl != nullptr) {
		HAL_ADC_Stop_DMA (mPtrAdcHndl) ;
		if (HAL_ADC_Start_DMA(mPtrAdcHndl, inBuf, inSize) == HAL_OK) retValue = true ;
	}

	return retValue ;
}
//--------------------------------------------------------
/*!
 * \brief Запуск преобразования с получением данных через DMA
 * @param inBuf Контейнер куда будут записаны считанные данные
 * @return true при успешном запуске
 */
bool TStmAdc::startConversationDMA (std::vector<uint16_t>& inBuf)
{
	return startConversationDMA (reinterpret_cast <uint32_t *> (inBuf.data ()), static_cast <uint32_t> (inBuf.size ())) ;
}
//--------------------------------------------------------
} /* namespace stm */
