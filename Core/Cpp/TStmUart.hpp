/*
 * TStmUart.hpp
 *
 *  Created on: Feb 25, 2021
 *      Author: AAL
 */

#ifndef CPP_TSTMUART_HPP_
#define CPP_TSTMUART_HPP_

#include <vector>

#include "usart.h"

namespace stm {

constexpr uint32_t defUartTimeout = 100 ;			///< таймоут для работы функций без прерываний

class TStmUart {
private:
	UART_HandleTypeDef* mPtrHndl { nullptr } ;		///< Хендл устройства
public:
	TStmUart(UART_HandleTypeDef *);
	virtual ~TStmUart() = default;

	bool transmit (uint8_t *, uint32_t) ;			///< Передача буфера
	bool transmit (std::vector<uint8_t> &) ;		///< Передача буфера

	bool receiveIT (uint8_t *, uint32_t) ;			///< Приём буфера по прерыванию
	bool receiveIT (std::vector<uint8_t> &) ;		///< Приём буфера по прерыванию

	bool receiveDMA (uint8_t *, uint32_t) ;			///< Приём буфера по DMA
	bool receiveDMA (std::vector<uint8_t> &) ;		///< Приём буфера по DMA
	bool transmitDMA (uint8_t *, uint32_t) ;		///< Передача буфера по DMA
	bool transmitDMA (std::vector<uint8_t> &) ;		///< Передача буфера по DMA
	bool stopDMA () ;								///< Остановка работы DMA
};

} /* namespace stm */

#endif /* CPP_TSTMUART_HPP_ */
