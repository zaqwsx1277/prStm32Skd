/*
 * TUnitOpen.hpp
 *
 *  Created on: 15 февр. 2021 г.
 *      Author: energia
 */

#ifndef CPP_TUNITOPEN_HPP_
#define CPP_TUNITOPEN_HPP_

#include <memory>

#include <TUnit.hpp>
#include "TStmGpioInput.hpp"

namespace app {

/*!
 * \brief Класс для получения значения состояния крышки блока.
 * \details Сначала сделал, и только сделав понял, что для этого типа какая в пень разница, дрожит кнопка или нет?
 * \todo Сделать запись события об изменении состояния концевика на флеш
 */

class TUnitOpen: public app::TUnit {
private:
														///< Данные переменные нужны т.к. запрос состояния концевика может придти в любое время если дребезг контактов не устранён, то х.з что получим
	GPIO_PinState mPrevData { GPIO_PIN_RESET } ;		///< Предыдущее состояние концевика
	GPIO_PinState mData { GPIO_PIN_RESET } ;			///< Текущее состояние концевика
public:
	std::unique_ptr <stm::TStmGpioInput> mPtrUnit { nullptr } ;		///< Указатель на класс обрабатывающий данные с концевика

	TUnitOpen (GPIO_TypeDef*, uint16_t);
	virtual ~TUnitOpen() = default ;

	unitState process () ;								///< Формирование состояния концевика в основном менеджере
	void setProcess (uint32_t inData = 0) { mProcessData = inData ; } 	///< Установка флага выполнения

	bool getData () ;									///< Получение текущего состояния концевика
	unitState checkBounce () ; 							///< Устранение дребезга контактов
};

} /* namespace stm */

#endif /* CPP_TUNITOPEN_HPP_ */
