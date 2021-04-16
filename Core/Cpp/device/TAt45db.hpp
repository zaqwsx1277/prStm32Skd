/*
 * TAt45db.hpp
 *
 *  Created on: 9 апр. 2021 г.
 *      Author: AAL
 */

#ifndef CPP_DEVICE_TAT45DB_HPP_
#define CPP_DEVICE_TAT45DB_HPP_

#include "TStmSpi.hpp"

/*!
 * Класс для работы с чипами флеш-памяти AT45DB...
 */
namespace device {

class TAt45db {
private:
	stm::TStmSpi *mPtrDev { nullptr } ;			// Указатель на класс обслуживающий
public:
	TAt45db();
	virtual ~TAt45db();
};

} /* namespace device */

#endif /* CPP_DEVICE_TAT45DB_HPP_ */
