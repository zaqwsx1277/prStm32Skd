/*
 * TComProtocolA2.hpp
 *
 *  Created on: 18 мар. 2021 г.
 *      Author: energia
 */

#ifndef CPP_COMPROTOCOL_TCOMPROTOCOLA2_HPP_
#define CPP_COMPROTOCOL_TCOMPROTOCOLA2_HPP_

#include <TComProtocol.hpp>

namespace comProtocol {

/// Класс для обработки команды получения сервисной информации с контроллера (команда 0xA2)
class TComProtocol_A2: public TComProtocol {
public:
	TComProtocol_A2() = default ;
	virtual ~TComProtocol_A2() = default ;

	void process (const std::vector<uint8_t> &, std::vector<uint8_t> &, const typeErr) ; ///< Формирование ответа на команду
};

} /* namespace comProtocol */

#endif /* CPP_COMPROTOCOL_TCOMPROTOCOLA2_HPP_ */
