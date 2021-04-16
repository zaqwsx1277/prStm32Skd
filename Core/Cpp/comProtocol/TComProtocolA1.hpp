/*
 * TComProtocolA1.hpp
 *
 *  Created on: 18 мар. 2021 г.
 *      Author: AAL
 */

#ifndef CPP_COMPROTOCOL_TCOMPROTOCOLA1_HPP_
#define CPP_COMPROTOCOL_TCOMPROTOCOLA1_HPP_

#include <TComProtocol.hpp>

namespace comProtocol {

/*!
 * Зарезервированный класс для отсылки команды о перезапуске контроллера (Команда 1В)
 */
class TComProtocol_A1: public TComProtocol {
public:
	TComProtocol_A1() = default ;
	virtual ~TComProtocol_A1() = default ;

	void process (const std::vector<uint8_t> &, std::vector<uint8_t> &, const typeErr) ;
};

} /* namespace comProtocol */

#endif /* CPP_COMPROTOCOL_TCOMPROTOCOLA1_HPP_ */
