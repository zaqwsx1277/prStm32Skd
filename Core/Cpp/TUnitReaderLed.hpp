/*
 * TUnitReaderLed.hpp
 *
 *  Created on: 16 апр. 2021 г.
 *      Author: energia
 */

#ifndef CPP_TUNITREADERLED_HPP_
#define CPP_TUNITREADERLED_HPP_

#include <TUnit.hpp>

namespace app {

class TUnitReaderLed: public TUnit {
public:
	TUnitReaderLed();
	virtual ~TUnitReaderLed();

	unitState process () { return usOk ; }
	void setProcess (uint32_t inData = 0) { mProcessData = inData ; } 	///< Установка флага выполнения
};

} /* namespace app */

#endif /* CPP_TUNITREADERLED_HPP_ */
