/*
 * TComProtocol01.hpp
 *
 *  Created on: 10 мар. 2021 г.
 *      Author: energia
 */

#ifndef CPP_COMPROTOCOL_TCOMPROTOCOL01_HPP_
#define CPP_COMPROTOCOL_TCOMPROTOCOL01_HPP_

#include <TComProtocol.hpp>


namespace comProtocol {

/*!
 * Класс обработки команды 0x01 "Смена адреса"
 */
class TComProtocol_01: public TComProtocol {
public:
	TComProtocol_01() = default  ;
	virtual ~TComProtocol_01() = default ;

	void process (const std::vector<uint8_t> &inBuf, std::vector<uint8_t> &outBuf, const typeErr) ;				///< Смена адреса и заводского номера устройства

};

} /* namespace comProtocol */

#endif /* CPP_COMPROTOCOL_TCOMPROTOCOL01_HPP_ */
