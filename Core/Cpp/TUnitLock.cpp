/*
 * TUnitLock.cpp
 *
 *  Created on: 25 мар. 2021 г.
 *      Author: energia
 */

#include <TUnitLock.hpp>

namespace app {
//------------------------------------------------------
/*!
 * @param inId Идентификатор устройства
 */
TUnitLock::TUnitLock (uint8_t inId) {
	mUnitId = inId ;
	mUnitType = utLock ;
	mUnitState = usOk ;
}
//------------------------------------------------------
} /* namespace app */
