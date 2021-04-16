/*
 * TComProtocolFD.hpp
 *
 *  Created on: 16 мар. 2021 г.
 *      Author: energia
 */

#ifndef CPP_COMPROTOCOL_TCOMPROTOCOLFD_HPP_
#define CPP_COMPROTOCOL_TCOMPROTOCOLFD_HPP_

#include <TComProtocol.hpp>

namespace comProtocol {

/// Класс обработки команды 0xFD "Установка системного времени"
class TComProtocol_FD: public TComProtocol {
public:
	TComProtocol_FD() = default ;
	virtual ~TComProtocol_FD() = default ;

	void process (const std::vector<uint8_t> &, std::vector<uint8_t> &, const typeErr) ; ///< обработка команды 0xFD "Установка системного времени"
};

} /* namespace comProtocol */

#endif /* CPP_COMPROTOCOL_TCOMPROTOCOLFD_HPP_ */
