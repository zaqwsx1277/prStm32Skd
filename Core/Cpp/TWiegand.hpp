/*
 * TWiegand.hpp
 *
 *  Created on: 2 февр. 2021 г.
 *      Author: energia
 */

#ifndef PRCOMMONCLASS_TWIEGAND_HPP_
#define PRCOMMONCLASS_TWIEGAND_HPP_

#include <memory>

#include "TSensors.hpp"

namespace reader {

constexpr size_t defMaxKeyLength = 26 ;	// Максимально возможная длина ключа
constexpr uint16_t defStopPeriod = 250 ; // Длительность периода ожидания конца посылки

/*!
 * \brief Возможные типы протоколов
 */
enum protocolType {
	wiegandUnknown = 0,
	wiegand26 = 26,			//!< wiegand26 (пока реализован только этот)
	wiegand33 = 33,			//!< wiegand33
	wiegand34 = 34,			//!< wiegand34
	wiegand35 = 35,			//!< wiegand35
	wiegand37 = 37,			//!< wiegand37
	wiegand40 = 40,			//!< wiegand40
	wiegand42 = 42 			//!< wiegand42
};

/*!
 * \brief Класс работы со считывателем по протоколу wiegand
 */
class TWiegand : public sensors::TSensors <uint64_t> {
private:
	uint8_t mPosCount { 0 } ;			///< Номер текущего принятого бита
	protocolType mProtocolType { wiegand26 } ; ///< Версия протокола
	uint32_t mTimeFinish { 0 } ;		///< Время для подсчёта периода окончания посылки defStopPeriod

public:
	TWiegand();
	virtual ~TWiegand() = default ;

	void get (uint8_t) ;				///< Добавление очередного бита в mValue
	protocolType checkProtocol () ;		///< Проверка типа протокола
	bool chechParity () ;				///< Проверка паритета
	bool checkFinish () ;				///< Проверка периода окончания посылки

	void getData () {} ;
	sensors::sensorState reset () ; ///< Сброс считывателя в исходное состояние
	uint64_t getValue () ;				///< Получение значение ключа прочитанного со считывателя

};

} /* namespace reader */

#endif /* PRCOMMONCLASS_TWIEGAND_HPP_ */
