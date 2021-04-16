/*
 * TUnit.cpp
 *
 *  Created on: Feb 8, 2021
 *      Author: energia
 */

#include "TUnit.hpp"
#include "TUnitReader.hpp"
#include "TUnitReaderAdc.hpp"
#include "TUnitOpen.hpp"
#include "TUnitTemperature.hpp"
#include "TUnitLock.hpp"
#include "TUnitDoorSensor.hpp"

namespace app {

////---------------------------------------------------------
///*!
// * @brief Конструктор
// * @param inUnitType Тип устройства
// * @attention Для всей системы внутренний идентификатор устройства должен быть уникальным и его необходимо установить методом setUnitNum
// */
//TUnit::TUnit(unitType inUnitType)
//{
//	fUnitType = inUnitType ;
//	fUnitNum = 0xFF ;
//}
//---------------------------------------------------------
/*!
 * \brief Получение типа устройства
 * @return Тип устройства
 */
unitType TUnit::getUnitType ()
{
	return mUnitType ;
}
//---------------------------------------------------------
/*!
 * @brief
 * Получение внутреннего идентификатора устройства
 * @return Внутрений идентификатор устройства
 */
uint8_t TUnit::getUnitNum ()
{
	return mUnitId;
}
//---------------------------------------------------------
/*!
 * @brief Создание указателя на наследуемый класс для работы патерна Фабрика
 * @param inType Тип создаваемого класса
 * @param inId Уникальный идентификатор создаваемого класса
 * @return Состояние созданного класса
 */
TUnit* TUnit::create (unitType inType, uint8_t inId)
{
	TUnit* retVal = nullptr ;

	switch (inType) {
	  case utReader:
		retVal = new TUnitReader (inId) ;
	  break;

	  case utSensorOpen:
		retVal = new TUnitOpen (GPIOA, GPIO_PIN_0) ;		///< todo И на хрена так делать???
	  break;

	  case utReaderAdc:
		retVal = new TUnitReaderAdc (common::defPtrReaderADC) ;
	  break;

	  case utSensorTemperature:
		retVal = new TUnitTemperature () ;
	  break ;

	  case utLock:
		retVal = new TUnitLock (inId) ;
	  break;

	  case utDoorSensor:
		retVal = new TUnitDoorSensor (inId) ;
	  break;

	  default:
	  break;
	}

	return retVal ;
}
//---------------------------------------------------------
/*!
 * @brief Установка состояния устройства
 * @param inState Устанавливаемое состояние
 */
void TUnit::setUnitState (unitState inState)
{
	mUnitState = inState ;
}
//---------------------------------------------------------
} /* namespace app */
