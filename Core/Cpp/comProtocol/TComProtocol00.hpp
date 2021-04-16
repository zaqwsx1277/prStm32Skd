/*
 * TComProtocol00.hpp
 *
 *  Created on: Mar 17, 2021
 *      Author: energia
 */

#ifndef CPP_TCOMPROTOCOL00_HPP_
#define CPP_TCOMPROTOCOL00_HPP_

#include <TComProtocol.hpp>

namespace comProtocol {

/// Класс обработки команды 0x00 "Смена серийного номера"
class TComProtocol_00: public TComProtocol {
public:
	TComProtocol_00() = default ;
	virtual ~TComProtocol_00() = default ;

	void process (const std::vector<uint8_t> &, std::vector<uint8_t> &, const typeErr) ;				///< Смена заводского номера устройства
};

} /* namespace comProtocol */

#endif /* CPP_TCOMPROTOCOL00_HPP_ */
