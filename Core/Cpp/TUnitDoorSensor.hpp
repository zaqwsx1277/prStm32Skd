/*
 * TUnitDoorSensor.hpp
 *
 *  Created on: 25 мар. 2021 г.
 *      Author: energia
 */

#ifndef CPP_TUNITDOORSENSOR_HPP_
#define CPP_TUNITDOORSENSOR_HPP_

#include <TUnit.hpp>

namespace app {

/*!
 * \brief Класс для работы с датчиком положения двери
 */
class TUnitDoorSensor: public TUnit {
public:
	TUnitDoorSensor (uint8_t) ;
	virtual ~TUnitDoorSensor() = default ;

	unitState process () { return usOk ; }
	void setProcess (uint32_t inData = 0) { mProcessData = inData ; } 	///< Установка флага выполнения
};

} /* namespace app */

#endif /* CPP_TUNITDOORSENSOR_HPP_ */
