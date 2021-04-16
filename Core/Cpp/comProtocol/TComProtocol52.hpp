/*
 * TComProtocol52.hpp
 *
 *  Created on: 22 мар. 2021 г.
 *      Author: energia
 */

#ifndef CPP_COMPROTOCOL_TCOMPROTOCOL52_HPP_
#define CPP_COMPROTOCOL_TCOMPROTOCOL52_HPP_

#include <TComProtocol.hpp>

namespace comProtocol {

/// Класс для обработки команды получения диагностики (команда 0x52)
class TComProtocol_52: public TComProtocol {
public:
	TComProtocol_52() = default ;
	virtual ~TComProtocol_52() = default ;

	void process (const std::vector<uint8_t> &, std::vector<uint8_t> &, const typeErr) ; ///< Формирование ответа на команду
};

} /* namespace comProtocol */

#endif /* CPP_COMPROTOCOL_TCOMPROTOCOL52_HPP_ */
