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

///  Всё, что казается работы с различными датчиками
namespace sensors {

 /*!
* \ingroup Перечисления
* Возможные состояния датчика
*/
enum sensorState {
		ssUnknown,		///< Хм.... Как я сюда попал? :(
		ssOK,			///< Нормальное состояние
		ssWait,			///< Ожидание
		ssReady,		///< Состояние готовности данных
		ssErrorHW,		///< Ошибка железа
		ssErrorSW,		///< Ощибка ПО
		ssErrorOther,	///< Прочая ошибка
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

	uint8_t getCRC (uint8_t *inData) { return 0 ; } ///< Вычисление CRC

public:
	TSensors () = default ;
	virtual ~TSensors() = default ;

	virtual void getData () = 0 ;			///< Получение данных непосредственно с датчика
	virtual T getValue () = 0 ;				///< Получение значения c датчика
	sensorState getState () { return mState ; } ///< Получение состояния датчика
	sensorState reset () { return ssUnknown ; }	///< Программый сброс датчика
};
//-------------------------------------------------------------------
} /* namespace sensors */

#endif /* PRCOMMONCLASS_TSENSORS_HPP_ */
