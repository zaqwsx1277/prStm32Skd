/*
 * TComProtocolFE.hpp
 *
 *  Created on: 9 мар. 2021 г.
 *      Author: energia
 */

#ifndef CPP_COMPROTOCOL_TCOMPROTOCOLFE_HPP_
#define CPP_COMPROTOCOL_TCOMPROTOCOLFE_HPP_

#include "TComProtocol.hpp"

namespace comProtocol {

/*!
 * Класс обработки команды 0хFE "Проверка адреса и заводского номера"
 */
class TComProtocol_FE : public TComProtocol {
public:
	TComProtocol_FE() : TComProtocol () { ; }
	virtual ~TComProtocol_FE() = default ;

	void process (const std::vector<uint8_t> &inBuf, std::vector<uint8_t> &outBuf, const typeErr) ;				///< Формирование ответа на Запрос проверки адреса и заводского номера устройства
};

} /* namespace comProtocol */

#endif /* CPP_COMPROTOCOL_TCOMPROTOCOLFE_HPP_ */
