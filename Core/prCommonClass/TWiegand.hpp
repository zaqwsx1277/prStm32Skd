/*
 * TWiegand.hpp
 *
 *  Created on: 2 февр. 2021 г.
 *      Author: energia
 */

#ifndef PRCOMMONCLASS_TWIEGAND_HPP_
#define PRCOMMONCLASS_TWIEGAND_HPP_

#include <array>
#include <memory>

#include "TSensors.hpp"

namespace wiegand {

constexpr size_t maxKeyLength = 6 ;	// Максимально возможная длина ключа

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
	wiegand42 = 42 			//!< wiegand42	(Более длинного ключа быть не может, т.к. в протоколе выделено только 5 байт)
};

/*!
 * \brief Класс работы со считывателем по протоколу wiegand
 */
class TWiegand : public sensors::TSensors <uint64_t> {
private:
	uint8_t mPosCount { 0 } ;			///< Номер текущего принятого бита
	protocolType mProtocolType { wiegand26 } ; ///< Версия протокола

public:
	TWiegand();
	virtual ~TWiegand();

	void get0 () ;						///< Добавление очередного бита 0 в mValue
	void get1 () ;						///< Добавление очередного бита 1 в mValue
	protocolType getProtocol () ;		///< Проверка типа протокола
	bool chechParity () ;				///< Проверка паритета

	void getData () {} ;
	sensors::sensorState reset () ;
	uint64_t getValue () ;				///< Получение значение ключа прочитанного со считывателя
};

} /* namespace protocol */

#endif /* PRCOMMONCLASS_TWIEGAND_HPP_ */
