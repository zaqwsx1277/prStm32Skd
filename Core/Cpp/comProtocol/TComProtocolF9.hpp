/*
 * TComProtocolF9.hpp
 *
 *  Created on: Mar 25, 2021
 *      Author: energia
 */

#ifndef CPP_COMPROTOCOL_TCOMPROTOCOLF9_HPP_
#define CPP_COMPROTOCOL_TCOMPROTOCOLF9_HPP_

#include <TComProtocol.hpp>

namespace comProtocol {

/// Класс для обработки команды Изменения длинны линии канала RS485 (команда 0xАВ)
class TComProtocol_F9: public TComProtocol {
public:
	TComProtocol_F9() = default ;
	virtual ~TComProtocol_F9() = default ;

	void process (const std::vector<uint8_t> &, std::vector<uint8_t> &, const typeErr) ; ///< Формирование ответа на команду
};

} /* namespace comProtocol */

#endif /* CPP_COMPROTOCOL_TCOMPROTOCOLF9_HPP_ */
