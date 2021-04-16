/*
 * TComProtocol13.hpp
 *
 *  Created on: Mar 26, 2021
 *      Author: energia
 */

#include "TComProtocol.hpp"

#ifndef CPP_COMPROTOCOL_TCOMPROTOCOL13_HPP_
#define CPP_COMPROTOCOL_TCOMPROTOCOL13_HPP_

namespace comProtocol {

/// Класс для обработки команды Закрытия всех дверей (аналог команды 0x05 0x00)
class TComProtocol_13 : public TComProtocol {
public:
	TComProtocol_13() = default ;
	virtual ~TComProtocol_13() = default ;

	void process (const std::vector<uint8_t> &, std::vector<uint8_t> &, const typeErr) ; ///< Формирование ответа на команду
};

} /* namespace comProtocol */

#endif /* CPP_COMPROTOCOL_TCOMPROTOCOL13_HPP_ */
