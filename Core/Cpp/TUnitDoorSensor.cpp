/*
 * TUnitDoorSensor.cpp
 *
 *  Created on: 25 мар. 2021 г.
 *      Author: energia
 */

#include <TUnitDoorSensor.hpp>

namespace app {
//------------------------------------------------------
/*!
 * @param inId Идентификатор устройства
 */
TUnitDoorSensor::TUnitDoorSensor (uint8_t inId) {
	mUnitId = inId ;
	mUnitType = utLock ;
	mUnitState = usOk ;
}
//--------------------------------------------------------


} /* namespace app */
