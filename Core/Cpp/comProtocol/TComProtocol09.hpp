/*
 * TComProtocol09.hpp
 *
 *  Created on: 9 апр. 2021 г.
 *      Author: energia
 */

#ifndef CPP_COMPROTOCOL_TCOMPROTOCOL09_HPP_
#define CPP_COMPROTOCOL_TCOMPROTOCOL09_HPP_

#include <TComProtocol.hpp>

namespace comProtocol {

/// Класс для обработки команды записи ключей в считыватели
class TComProtocol_09: public TComProtocol {
public:
	TComProtocol_09() = default ;
	virtual ~TComProtocol_09() = default ;

	void process (const std::vector<uint8_t> &, std::vector<uint8_t> &, const typeErr) ; ///< Формирование ответа на команду
};

} /* namespace comProtocol */

#endif /* CPP_COMPROTOCOL_TCOMPROTOCOL09_HPP_ */
