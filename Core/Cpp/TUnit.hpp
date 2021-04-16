/*
 * TUnit.hpp
 *
 *  Created on: Feb 8, 2021
 *      Author: energia
 */

#ifndef SRC_TUNIT_HPP_
#define SRC_TUNIT_HPP_

#include <stdint.h>
#include <vector>

namespace app {

/*!
 * \brief Возможные типы обслуживаемых устройств
 * \ingroup Перечисления
 */
enum unitType {
	utUnknown,
	utReader, 					///< Считыватель
	utReaderLed,				///< Индикатор на считывателе
	utReaderAdc,				///< Датчик контроля несанкционированного подключения к считывателю
	utButton, 					///< Кнопка
	utButtonLed,				///< Индикатор на кнопке
	utLock, 					///< Замок
	utDoorSensor,				///< Датчик двери
	utSensorOpen,				///< Датчик открытия крышки блока
	utSensorTemperature			///< Датчик температуры
} ;

/*!
 * \brief Возможные состояния обслуживаемого устройства
 * \ingroup Перечисления
 * \todo Спроектировал очевидную хуйню!!!
 */
enum unitState {
	usOk,
	usWait,						///< Для считывателя: ожидание поступления очередного бита
	usReady,  					///< Устройство готово к обработке данных
	usErrSw,  					///< Ошибка ПО
	usErrHw,   					///< Ошибка железа
	usCheckBounce,				///< Для кнопок проверка дребезга контактов
	usADCConversation,			///< АЦП выполняет конвертацию
	usOpen,						///< Устройство открыто
	usClose,					///< Устройство закрыто
	usUnknown
};

/*!
 * \brief Базовый класс для патерна типа Фабрика для работы с различными устройствами
 * \todo переделать реализацию патерна!!!
 */
class TUnit {
protected:
	unitType mUnitType { utUnknown }; 	///< Тип устройства
	unitState mUnitState { usUnknown } ;///< Состояние устройства
	uint8_t mUnitId { 0xFF } ;			///< Внутренний идентификатор устройства
	std::vector <uint8_t> mListExecute ; ///< Список исполнительных устройств
	uint8_t mRoomId { 0xFF } ;			///< Идентификатор помещения
	uint32_t mProcessData { 0 } ;				///< Дополнительные данные передаваемые для выполнения метода process ()

public:
	TUnit () = default ;
	virtual ~TUnit() = default ;

	static TUnit* create (unitType, uint8_t) ;	///< Создание указателя на наследуемый класс
	virtual unitState process () = 0 ;	///< Обработка состояния датчика
	virtual void setProcess (uint32_t inData = 0) = 0 ; 	///< Установка флага выполнения

	unitType getUnitType () ; 			///< Получение типа устройства
	uint8_t getUnitNum () ;				///< Получение внутреннего идентификатора устройства
	unitState getUnitState () { return mUnitState ; } ///< Получение текущего состояния устройства
	void setUnitState (unitState) ;		///< Установка состояния устройства
	void setRoomId (uint8_t inRoomId) { mRoomId = inRoomId ; } ///< Установка номера помещения
	uint8_t getRoomId () { return mRoomId ; } ///< Получение номера помещения
	void executeClear () ;				///< Очистка списка исполнительных устройств
	void executeAdd (uint8_t) ;			///< Добавление нового устройства
};

} /* namespace app */

#endif /* SRC_TUNIT_HPP_ */
