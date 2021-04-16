/*
 * TUnitTemperature.cpp
 *
 *  Created on: 24 февр. 2021 г.
 *      Author: energia
 */

#include <TUnitTemperature.hpp>
#include "TApplication.hpp"


namespace app {
//-------------------------------------------------------------
/*!
 * \brief Конструктор
 */
TUnitTemperature::TUnitTemperature()
{
	mCalibration = 8000 / (*mPtrTempCalibration110 - *mPtrTempCalibration30) ; // хммм!!!! А что будет, если эти калибровочные значения будут равны?
}
//-------------------------------------------------------------
/*!
 * \brief Получение текущей температуры процессора
 * @return Температура процессора умноженная на 100
 */
int16_t TUnitTemperature::getTemperature ()
{
	int16_t retValue { 0x7FFF } ;

	if (mTemperature != 0xFFFF) {
		double temp = mTemperature - *mPtrTempCalibration30 ;
		temp = temp * mCalibration + 3000 ;
		retValue = static_cast <int16_t> (temp) ;
	}

	return retValue ;
}
//-------------------------------------------------------------
} /* namespace app */
