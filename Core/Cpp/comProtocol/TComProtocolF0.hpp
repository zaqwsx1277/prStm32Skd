/*
 * TComProtocolF0.hpp
 *
 *  Created on: 5 мар. 2021 г.
 *      Author: energia
 */

#ifndef CPP_COMPROTOCOL_TCOMPROTOCOLF0_HPP_
#define CPP_COMPROTOCOL_TCOMPROTOCOLF0_HPP_

#include <TComProtocol.hpp>

namespace comProtocol {

class TComProtocol_F0: public TComProtocol {
private:
public:
	TComProtocol_F0 () : TComProtocol () { ; }
	virtual ~TComProtocol_F0() = default ;

	void process (const std::vector<uint8_t> &inBuf, std::vector<uint8_t> &outBuf, const typeErr) ;				///< Обработка полученной команды
};

} /* namespace stm */

#endif /* CPP_COMPROTOCOL_TCOMPROTOCOLF0_HPP_ */
