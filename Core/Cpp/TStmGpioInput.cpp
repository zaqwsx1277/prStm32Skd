/*
 * TStmGpioInput.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: AAL
 */

#include <TStmGpioInput.hpp>

namespace stm {

//----------------------------------------------------------------------------
TStmGpioInput::TStmGpioInput(GPIO_TypeDef* inGpioPort, uint16_t inGpioPin) : TStmGpio (inGpioPort, inGpioPin)
{

}
//----------------------------------------------------------------------------
GPIO_PinState TStmGpioInput::get ()
{
	return HAL_GPIO_ReadPin(mGpioPort, mGpioPin) ;
}
//----------------------------------------------------------------------------
bool TStmGpioInput::readData ()
{
	get () ;
	mState = gpOk ;
	return true ;
}
//----------------------------------------------------------------------------
/*!
 * \brief Устранение дребезга контактов
 * \details дребезг контактов будет устраняться до тех пор, пока в mCheckBounce не запишется значение 0xFFFF или 0x0000. В теории для этого достаточно не более 16 тиков
 * Данный метод желательно вызывать в обработчике прерываний по таймеры с тиков в 1мСек и т.о. дребезг будет устранен минимум за 16 тактов
 * @return Признак устранения дребезга контактов.
 */
bool TStmGpioInput::checkBounce ()
{
	bool retValue { false } ;

	switch (mCountTick) {
	  case 0 :
		mCheckBounce = 0x01 ;
		mCountTick++ ;
	  break ;

	  case 0xFF :					// За 256 тиков дребезг контактов так и не устранился
		mCountTick = 0 ;
		retValue = true ;
	  break;

	  default:
		if (get () == GPIO_PIN_SET) {
			mCheckBounce = mCheckBounce << 1 ;
			mCheckBounce |= 0x01 ;
		}
		  else mCheckBounce = mCheckBounce << 1 ;
		if (mCheckBounce == 0xFFFF || mCheckBounce == 0x0000) {  // Усё. Дребезг устранили
			mCountTick = 0 ;
			retValue = true ;
		}
		  else mCountTick ++ ;
	  break;
	}

	return retValue ;
}
//----------------------------------------------------------------------------
} /* namespace stm */
