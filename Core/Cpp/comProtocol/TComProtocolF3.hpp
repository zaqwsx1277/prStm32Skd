/*
 * TComProtocolF3.hpp
 *
 *  Created on: Apr 1, 2021
 *      Author: energia
 */

#ifndef CPP_COMPROTOCOL_TCOMPROTOCOLF3_HPP_
#define CPP_COMPROTOCOL_TCOMPROTOCOLF3_HPP_

#include <TComProtocol.hpp>

namespace comProtocol {

class TComProtocol_F3: public TComProtocol {
public:
	TComProtocol_F3() = default ;
	virtual ~TComProtocol_F3() = default ;

	void process (const std::vector<uint8_t> &, std::vector<uint8_t> &, const typeErr) ; ///< Формирование ответа на команду
};

} /* namespace comProtocol */

#endif /* CPP_COMPROTOCOL_TCOMPROTOCOLF3_HPP_ */
