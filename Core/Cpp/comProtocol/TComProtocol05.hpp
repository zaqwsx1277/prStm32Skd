/*
 * TComProtocol05.hpp
 *
 *  Created on: 26 мар. 2021 г.
 *      Author: energia
 */

#ifndef CPP_COMPROTOCOL_TCOMPROTOCOL05_HPP_
#define CPP_COMPROTOCOL_TCOMPROTOCOL05_HPP_

#include <TComProtocol.hpp>

namespace comProtocol {

/// Класс для обработки команды открытия дверей
class TComProtocol_05: public TComProtocol {
public:
	TComProtocol_05() = default ;
	virtual ~TComProtocol_05() = default ;

	void process (const std::vector<uint8_t> &, std::vector<uint8_t> &, const typeErr) ; ///< Формирование ответа на команду
};

} /* namespace comProtocol */

#endif /* CPP_COMPROTOCOL_TCOMPROTOCOL05_HPP_ */
