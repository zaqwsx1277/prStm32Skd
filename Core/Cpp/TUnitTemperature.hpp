/*
 * TUnitTemperature.hpp
 *
 *  Created on: 24 февр. 2021 г.
 *      Author: AAL
 */

#ifndef CPP_TUNITTEMPERATURE_HPP_
#define CPP_TUNITTEMPERATURE_HPP_

#include "TUnit.hpp"
#include "TSensors.hpp"

namespace app {

class TUnitTemperature: public TUnit {
private:
	double mCalibration { 0 } ;				///< Калибровочное значение для вычисления температуры
	uint16_t mTemperature { 0xFFFF } ;		///< Значение температуры прочитанное с АЦП

	const uint16_t *mPtrTempCalibration30 { ((uint16_t *)  0x1FFF7A2Cu) } ; ///< Адрес калибровочного значения для температуры 30 градусов
	const uint16_t *mPtrTempCalibration110  { ((uint16_t *) 0x1FFF7A2Eu)  } ; ///< Адрес калибровочного значения для температуры 110 градусов

public:
	TUnitTemperature() ;
	virtual ~TUnitTemperature() = default ;

	unitState process () { return usOk ; }
	void setProcess (uint32_t inData = 0) { mProcessData = inData ; } 	///< Установка флага выполнения

	void setTemperature (const uint16_t inTemp) { mTemperature = inTemp ; } ///<  Фиксация полученных данных с АЦП
	int16_t getTemperature () ;				///< Получение текущей температуры процессора
};

} /* namespace app */

#endif /* CPP_TUNITTEMPERATURE_HPP_ */
