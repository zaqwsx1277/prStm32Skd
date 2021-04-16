/*
 * TUnitOpen.cpp
 *
 *  Created on: 15 февр. 2021 г.
 *      Author: AAL
 */

#include "TUnitOpen.hpp"
#include "TState.hpp"
#include "TApplication.hpp"

namespace app {

//-------------------------------------------------------------
/*!
 * @param inGpioPort Порт на котором сидит концевик
 * @param inGpioPin PIN на котором сидит концевик
 * \todo Переделать чтение начального состояния концевиков
 */
TUnitOpen::TUnitOpen (GPIO_TypeDef* inGpioPort, uint16_t inGpioPin) {
	mPtrUnit = std::make_unique <stm::TStmGpioInput> (inGpioPort, inGpioPin) ;
	mData = mPtrUnit -> get() ;				// Запоминаем начальное состояние концевика
	mUnitId = common::defPort::portOpen ;
	mUnitState = usOk ;
}
//-------------------------------------------------------------
/*!
 * @return текущее состояние датчика крышки блока
 */
unitState TUnitOpen::process ()
{
	unitState retValue { mUnitState } ;

	switch (mUnitState) {
	  case usOk:{
		GPIO_PinState data = mPtrUnit -> get() ;	// Проверяем сработал ли концевик и если состояние изменилось, то запускаем устранение дребезга контактов
		if (data != mData) {
			mUnitState = usCheckBounce ;
			retValue = usCheckBounce ;
		}
	  }
	  break;

	  default:
	  break;
	}
	return retValue ;
}
//-------------------------------------------------------------
/*!
 * @return Если крышка открыта, то возвращается true
 */
bool TUnitOpen::getData ()
{
	bool retValue  { false } ;

	if (mData == GPIO_PIN_SET) retValue = true ;

	return retValue ;
}
//-------------------------------------------------------------
/*!
 * @return текущее состояние кнопки
 */
unitState TUnitOpen::checkBounce ()
{
	unitState retValue { mUnitState } ;

	if (mUnitState == usCheckBounce) {				// Устраняем дребезг контактов кнопки и когда он устранён, то фиксируем значение
		if (mPtrUnit -> checkBounce()) {
			mData = mPtrUnit -> get() ;
			if (mPrevData != mData) {				// Если состояние изменилось, то записываем это событие в очередь на отправку
				common::defState sensorOpenState { common::defState::unknown } ;
				mPrevData = mData ;
				if (mData == GPIO_PIN_SET) {
					sensorOpenState  = common::defState::sensorOpen ;
				}
				  else sensorOpenState  = common::defState::sensorClose ;
				std::shared_ptr <TState> ptrStateUnit (new TState (common::defPort::portOpen, sensorOpenState )) ;
				application -> setQueueTxElement (ptrStateUnit) ;
			}
			mUnitState = usOk ;						// переводим в нормальный режим
			retValue = usOk ;
		}
	}

	return retValue ;
}
//-------------------------------------------------------------
} /* namespace stm */
