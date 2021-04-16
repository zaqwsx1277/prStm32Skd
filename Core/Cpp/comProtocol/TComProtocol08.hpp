/*
 * TComProtocol08.hpp
 *
 *  Created on: Mar 26, 2021
 *      Author: energia
 */

#ifndef CPP_COMPROTOCOL_TCOMPROTOCOL08_HPP_
#define CPP_COMPROTOCOL_TCOMPROTOCOL08_HPP_

#include <TComProtocol.hpp>

namespace comProtocol {

/// Класс для обработки команды разблокировки дверей
class TComProtocol_08: public TComProtocol {
public:
	TComProtocol_08() = default ;
	virtual ~TComProtocol_08() = default ;

	void process (const std::vector<uint8_t> &, std::vector<uint8_t> &, const typeErr) ; ///< Формирование ответа на команду
};

} /* namespace comProtocol */

#endif /* CPP_COMPROTOCOL_TCOMPROTOCOL08_HPP_ */
