/*
 * TUnitReaderAdc.cpp
 *
 *  Created on: Feb 17, 2021
 *      Author: AAL
 */
#include <algorithm>

#include "TUnitReaderAdc.hpp"
#include "TUnitReader.hpp"
#include "TUnitTemperature.hpp"
#include "TApplication.hpp"

namespace app {
//------------------------------------------------------------------------
TUnitReaderAdc::TUnitReaderAdc(ADC_HandleTypeDef* inHndl) : TUnit (), stm::TStmAdc (inHndl)
{
	mUnitState = usOk ;
	mUnitType = utReaderAdc ;
}
//------------------------------------------------------------------------
/*!
 * \brief Обработка состояния АЦП всех считывателей и датчика температуры
 * @return
 */
unitState TUnitReaderAdc::process ()
{
	unitState retValue { mUnitState } ;

	switch (mUnitState) {
	  case usReady:			// Распихиваем полученные значения по классам считывателей
		static_cast <app::TUnitReader *> (application -> mListUnits [common::defPort::portRd0]) -> setAdc (mBufData.at (0)) ;
		static_cast <app::TUnitReader *> (application -> mListUnits [common::defPort::portRd1]) -> setAdc (mBufData.at (1)) ;
		static_cast <app::TUnitReader *> (application -> mListUnits [common::defPort::portRd2]) -> setAdc (mBufData.at (2)) ;
		static_cast <app::TUnitReader *> (application -> mListUnits [common::defPort::portRd3]) -> setAdc (mBufData.at (3)) ;

		static_cast <app::TUnitTemperature *> (application -> mListUnits [common::defPort::portTemp]) -> setTemperature(mBufData.at (4)) ;
	  break;

	  default:
	  break;
	}

	return retValue ;
}
//------------------------------------------------------------------------
/*!
 * \brief Запуск АЦП
 */
void TUnitReaderAdc::startConversation ()
{
//	if (mUnitState == usOk || mUnitState == usReady) {
		this -> startConversationDMA (mBufData) ;
		mUnitState = usADCConversation ;
//	}
}
//------------------------------------------------------------------------
} /* namespace app */
