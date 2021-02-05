/*
 * TSensors.hpp
 *
 *  Created on: Oct 29, 2020
 *      Author: AAL
 *
 *      Базовый класс для работы с датчиками
 */

#ifndef PRCOMMONCLASS_TSENSORS_HPP_
#define PRCOMMONCLASS_TSENSORS_HPP_

#ifdef STM32F103xB
#include "stm32f1xx_hal.h"
#endif

#ifdef STM32F429xx
#include "stm32f4xx_hal.h"
#endif

#ifdef STM32F407xx
#include "stm32f4xx_hal.h"
#endif

#include "rtc.h"

namespace sensors {

/*!
 *  Возможные состояния датчика
 */
enum sensorState {
		ssUnknown,
		ssOK,
		ssWait,
		ssReady,
		ssErrorHW,
		ssErrorSW,
		ssErrorOther,
		ssCount}  ;


constexpr uint32_t defSensorTimeOut = 100 ; 		///< Таймaут получения данных с датчика

//--------------------------------------------------------------------
/*!
 * 	Шаблон для работы с различными датчиками
 */
template <typename T>
class TSensors {
protected:
	T mValue ;								///< Обработанное значение датчиков

	sensorState mState { ssUnknown } ;		///< Состояние датчика
	RTC_DateTypeDef mDate { 0, } ;			///< Дата и время последнего обновления данный прочитанных с датчика
	RTC_TimeTypeDef mTime { 0, } ;

	virtual uint8_t getCRC (uint8_t *) = 0 ;///< Вычисление CRC

public:
	TSensors () = default ;
	~TSensors() = default ;

	virtual void getData () = 0 ;			///< Получение данных непосредственно с датчика
	virtual T getValue () = 0 ;				///< Получение значения c датчика
	sensorState getState () { return mState ; } ///< Получение состояния датчика
	virtual sensorState reset () = 0 ;		///< Программый сброс датчика
};
//-------------------------------------------------------------------
} /* namespace sensors */

#endif /* PRCOMMONCLASS_TSENSORS_HPP_ */
