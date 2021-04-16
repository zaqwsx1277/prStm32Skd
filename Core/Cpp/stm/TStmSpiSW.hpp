/*
 * TStmSpiNSS.hpp
 *
 *  Created on: Mar 12, 2021
 *      Author: AAL
 */

#ifndef CPP_STM_TSTMSPISW_HPP_
#define CPP_STM_TSTMSPISW_HPP_

#include "TStmSpi.hpp"
#include "TStmGpioOutput.hpp"

namespace stm {

/*!
 * \brief Класс для работы с портом SPI c софтовым управлением
 */
class TStmSpiSW: public TStmSpi {
private:
	TStmGpioOutput *mPtrControl { nullptr } ;	// Указатель на GPIO для управления работой по SPI
public:
	TStmSpiSW(SPI_HandleTypeDef *inHndl, TStmGpioOutput *) ; // : TStmSpi (inHndl) { ; }
	virtual ~TStmSpiSW() = default ;
};

} /* namespace stm */

#endif /* CPP_STM_TSTMSPISW_HPP_ */
