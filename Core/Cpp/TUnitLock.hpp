/*
 * TUnitLock.hpp
 *
 *  Created on: 25 мар. 2021 г.
 *      Author: energia
 */

#ifndef CPP_TUNITLOCK_HPP_
#define CPP_TUNITLOCK_HPP_

#include <TUnit.hpp>

namespace app {

/*!
 * \brief Класс для работы с замком
 */
class TUnitLock: public TUnit {
public:
	TUnitLock(uint8_t) ;
	virtual ~TUnitLock() = default ;

	unitState process () { return usOk ; }
	void setProcess (uint32_t inData = 0) { mProcessData = inData ; } 	///< Установка флага выполнения
};

} /* namespace app */

#endif /* CPP_TUNITLOCK_HPP_ */
