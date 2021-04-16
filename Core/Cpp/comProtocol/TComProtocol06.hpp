/*
 * TComProtocol06.hpp
 *
 *  Created on: Mar 26, 2021
 *      Author: energia
 */

#ifndef CPP_COMPROTOCOL_TCOMPROTOCOL06_HPP_
#define CPP_COMPROTOCOL_TCOMPROTOCOL06_HPP_

#include <TComProtocol.hpp>

namespace comProtocol {

/// Класс для обработки команды Закрытия дверей
class TComProtocol_06: public TComProtocol {
public:
	TComProtocol_06() = default ;
	virtual ~TComProtocol_06() = default ;

	void process (const std::vector<uint8_t> &, std::vector<uint8_t> &, const typeErr) ; ///< Формирование ответа на команду
};

} /* namespace comProtocol */

#endif /* CPP_COMPROTOCOL_TCOMPROTOCOL06_HPP_ */
