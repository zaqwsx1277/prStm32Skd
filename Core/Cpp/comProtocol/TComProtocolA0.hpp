/*
 * TComProtocolA0.hpp
 *
 *  Created on: 15 мар. 2021 г.
 *      Author: energia
 */

#ifndef CPP_COMPROTOCOL_TCOMPROTOCOLA0_HPP_
#define CPP_COMPROTOCOL_TCOMPROTOCOLA0_HPP_

#include <TComProtocol.hpp>

namespace comProtocol {

/*!
 * \ingroup Перечисления
 * \brief Возможные типы команд сброса в заводское состояние
*/
enum class defTypeComA0 : uint8_t {
	full = 0, key0 = 1, key1 = 2, key2 = 3, key3 = 4, programm = 5, sysInfo = 6
} ;

/*!
 * Класс обрабатывающий сброс контроллера в заводские установки
 * \attention Случайно пойманная эта команда может привести к полной неработоспособности системы
 * \todo Приделать подтверждение сброса
 */
class TComProtocol_A0: public TComProtocol {
public:
	TComProtocol_A0() = default ;
	virtual ~TComProtocol_A0() = default ;

	void process (const std::vector<uint8_t> &, std::vector<uint8_t> &, const typeErr) ;				///< Полное стирание флешки AT45DB321D
};

} /* namespace comProtocol */

#endif /* CPP_COMPROTOCOL_TCOMPROTOCOLA0_HPP_ */
