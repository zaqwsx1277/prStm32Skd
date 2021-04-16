/*
 * TState.cpp
 *
 *  Created on: 15 февр. 2021 г.
 *      Author: energia
 */

#include "TState.hpp"
#include "TApplication.hpp"


namespace app {
//-----------------------------------------------------------------------------
/*!
 * @param inId	Внутренний номер устройства
 * @param inState ID события
 * @param inRoomId Номер помещения
 * @param inKey Ключ прочитанный считывателем
 */
TState::TState(uint8_t inId, common::defState inState, uint8_t inRoomId, common::typeKey inKey)
{
	mUnitId = inId ;
	mUnitState = inState ;
	mRoomId = inRoomId ;
	mKey = inKey ;

	HAL_RTC_GetTime(&hrtc, &mTime, RTC_FORMAT_BIN) ;
	HAL_RTC_GetDate(&hrtc, &mDate, RTC_FORMAT_BIN) ;
}
//-----------------------------------------------------------------------------
/*!
 * @return Рассчитанное время события
 */
uint16_t TState::getTime ()
{
	uint16_t retValue { 0 } ;


	return retValue ;
}
//-----------------------------------------------------------------------------

} /* namespace app */
