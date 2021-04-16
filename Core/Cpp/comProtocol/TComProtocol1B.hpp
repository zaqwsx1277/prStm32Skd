/*
 * TComProtocol1B.hpp
 *
 *  Created on: 18 мар. 2021 г.
 *      Author: AAL
 */

#ifndef CPP_COMPROTOCOL_TCOMPROTOCOL1B_HPP_
#define CPP_COMPROTOCOL_TCOMPROTOCOL1B_HPP_

#include <TComProtocol.hpp>

namespace comProtocol {

/*!
 * \brief Класс обработки команды на перезапуск контроллера
 */
class TComProtocol_1B: public TComProtocol {
public:
	TComProtocol_1B() = default ;
	virtual ~TComProtocol_1B() = default ;

	void process (const std::vector<uint8_t> &, std::vector<uint8_t> &, const typeErr) ;
};

} /* namespace comProtocol */

#endif /* CPP_COMPROTOCOL_TCOMPROTOCOL1B_HPP_ */
