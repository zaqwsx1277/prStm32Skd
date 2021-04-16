/*
 * TComProtocol07.hpp
 *
 *  Created on: Mar 26, 2021
 *      Author: energia
 */

#ifndef CPP_COMPROTOCOL_TCOMPROTOCOL07_HPP_
#define CPP_COMPROTOCOL_TCOMPROTOCOL07_HPP_

#include <TComProtocol.hpp>

namespace comProtocol {

/// Класс для обработки команды блокировки дверей
class TComProtocol_07: public TComProtocol {
public:
	TComProtocol_07() = default;
	virtual ~TComProtocol_07() = default ;

	void process (const std::vector<uint8_t> &, std::vector<uint8_t> &, const typeErr) ; ///< Формирование ответа на команду
};

} /* namespace comProtocol */

#endif /* CPP_COMPROTOCOL_TCOMPROTOCOL07_HPP_ */
