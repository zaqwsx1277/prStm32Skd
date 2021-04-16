/*
 * TState.hpp
 *
 *  Created on: 15 февр. 2021 г.
 *      Author: AAL
 */

#ifndef CPP_TSTATE_HPP_
#define CPP_TSTATE_HPP_

#include <stdint.h>

#include <rtc.h>

#include "TCommon.hpp"

namespace app {

/*!
 * \brief Класс описания произошедшего события
 */
class TState {
private:
	uint8_t mUnitId { 0xFF } ;						///< Идентификатор устройства
	common::defState mUnitState { common::defState::unknown } ;	///< Состояние
	RTC_DateTypeDef mDate {0};						///< Дата возникновения события
	RTC_TimeTypeDef mTime {0};						///< Время возникновения события
	uint8_t mRoomId { 0xff } ;						///< Идентификатор помещения
	common::typeKey mKey { 0xFFFF } ;				///< Прочитанный ключ
	common::defTxState mTxState { common::stTxReady } ; ///< Состояние отправки сообщения

public:
	TState(uint8_t inUnitId, common::defState inUnitState, uint8_t inRoomId = 0xFF, common::typeKey inKey = 0xFFFF);
	virtual ~TState() = default ;

	uint8_t getUintId () { return mUnitId ; } 		///< Получение идетнификатора устройства
	common::defState getState () { return mUnitState ; } ///< Получение типа  события
	common::defTxState getTxState () { return mTxState ; } ///< Получение статуса передачи сообщения
	void setTxState (common::defTxState inTxState) { mTxState = inTxState ; } ///< Установка статуса передачи сообщения
	uint8_t getRoomId () { return mRoomId ; } 		///< Получение идентификатора помещения
	common::typeKey getKey () { return mKey ; } 	///< Получение ключа
	uint16_t getTime () ;							///< Получение времени события в секундах с точностью до 2х сек.
};

} /* namespace app */

#endif /* CPP_TSTATE_HPP_ */
