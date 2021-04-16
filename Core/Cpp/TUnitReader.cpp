/*
 * TUintReader.cpp
 *
 *  Created on: 11 февр. 2021 г.
 *      Author: AAL
 */

#include "TUnitReader.hpp"
#include "TCommon.hpp"
#include "TState.hpp"

namespace app {

//-------------------------------------------------------------
/*!
 * @param inId Идентификатор устройства
 */
TUnitReader::TUnitReader(uint8_t inId) {
	mPtrUnit = std::make_unique <reader::TWiegand> () ;
	mUnitId = inId ;
	mUnitType = utReader ;
	mUnitState = usOk ;
	mAdcList.resize(common::defReaderADCCount + 2) ;				// Сразу задаём максимальный размер контейнеров, т.к. stl через жопу работает с памятью.
	mKeyList.reserve(common::defKeyNumber) ;

	for (uint32_t i = 0; i < common::defKeyNumber; ++i) {			// Читаем из FLASH ключи для считывателя
		uint32_t port = inId - common::portRd0 ;
		uint32_t key = *((uint32_t *) (0x08000000 + (common::defFlashReader - port) * common::defFlashPageSize) + i * common::defKeySize) ;
		if (key != 0xFFFFFFFF)
			mKeyList.insert(key) ;
	}
}
//--------------------------------------------------------------
/*!
 * @brief Обработка состояний считывателя
 * @return Обработанное состояние считывателя
 */
unitState TUnitReader::process ()
{
	unitState retVal = mUnitState ;
	switch (mPtrUnit -> getState()) {
	  case sensors::ssReady: {				  // пойман признак завершения посылки
		  uint64_t key = mPtrUnit -> getValue () ;
		  if (key == 0xFFFFFFFFFFFFFFFF) {			  // Обрабатываем полученный ключ
			  if (mKeyList.find (static_cast <uint16_t> (key)) != mKeyList.end()) {
				  std::shared_ptr <TState> ptrStateUnit (new TState (this -> mUnitId, common::defState::doorOpen, mRoomId)) ;
// Здесь нужно открыть дверь
				  mPtrUnit -> reset () ;
				  mUnitState = usOk ;
			  }
			    else {							// Ощибка при чтении ключа
			      std::shared_ptr <TState> ptrStateUnit (new TState (this -> mUnitId, common::defState::doorErrKey, mRoomId)) ;
			    }
		  }
	  }
	  break;

	  default:
	  break;
	}
	return retVal ;
}
//--------------------------------------------------------------
/*!
 * @brief Добавление значения считанного с АЦП
 * @param inValue
 */
void TUnitReader::setAdc (uint16_t inValue)
{
	mAdcList.push_back(inValue) ;
	if (mAdcList.size() > common::defReaderADCCount) {
		mAdcList.erase(mAdcList.begin()) ;
	}
//		mAdcList.pop_back() ;	// Размер контейнера всегда должен быть не больше common::defReaderADCCount
}
//--------------------------------------------------------------
/*!
 * @brief Получение усреднённого значений данных прочитанных с АЦП
 * @return Возвращается среднее значение. Если с начала работы не прошло defReaderADCCount, то возвращается 0xFFFF
 * @details Значение считанное с АЦП усредняется за defReaderADCCount
 */
uint16_t TUnitReader::getMeanAdc ()
{
	uint64_t retValue { 0 } ;

	if (mAdcList.size () < common::defReaderADCCount) retValue = 0xFFFF ;
	  else {
		for (auto it = mAdcList.begin (); it != mAdcList.end (); ++it) {
			retValue += *it ;
		}
		retValue /= mAdcList.size() ;
	  }

	return static_cast <uint16_t> (retValue) ;
}
//--------------------------------------------------------------
} /* namespace app */
