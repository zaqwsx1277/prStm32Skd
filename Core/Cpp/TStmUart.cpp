/*
 * TStmUart.cpp
 *
 *  Created on: Feb 25, 2021
 *      Author: AAL
 */

#include <TStmUart.hpp>

namespace stm {
//----------------------------------------------------------
/*!
 * @param inHndl Указатель на хендл для работы с устройством
 */
TStmUart::TStmUart(UART_HandleTypeDef *inHndl) {
	mPtrHndl = inHndl ;
}
//----------------------------------------------------------
/*!
 * @param inBuf Указатель на передаваемый буфер
 * @param inBufLen Длина передаваемого буфера
 * @return При успешном запуске передачи возвращается true
 */
bool TStmUart::transmitDMA (uint8_t *inBuf, uint32_t inBufLen)
{
	return (HAL_UART_Transmit_DMA(mPtrHndl, inBuf, inBufLen) == HAL_OK) ;
}
//----------------------------------------------------------
/*!
 * @param inBuf Ссылка на передаваемые данные
 * @return При успешном запуске передачи возвращается true
 */
bool TStmUart::transmitDMA (std::vector<uint8_t> &inBuf)
{
	return transmitDMA (inBuf.data (), inBuf.size()) ;
}
//----------------------------------------------------------
/*!
 * @param inBuf Указатель на буфер для приёма данных
 * @param inBufLen Длинна буфера для приёма данных
 * @return При успешном запуске передачи возвращается true
 */
bool TStmUart::receiveDMA (uint8_t * inBuf, uint32_t inBufLen)
{
	return (HAL_UART_Receive_DMA(mPtrHndl, inBuf, inBufLen) == HAL_OK) ;
}
//----------------------------------------------------------
/*!
 * @param inBuf Ссылка на контейнер для принятых данных
 * @return При успешном запуске передачи возвращается true
 */
bool TStmUart::receiveDMA (std::vector<uint8_t> &inBuf)
{
	return receiveDMA (inBuf.data (), inBuf.size()) ;
}
//----------------------------------------------------------
/*!
 * @param inBuf Указатель на буфер для приёма данных
 * @param inBufLen Длинна буфера для приёма данных
 * @return При успешном запуске передачи возвращается true
 */
bool TStmUart::receiveIT (uint8_t * inBuf, uint32_t inBufLen)
{
	return (HAL_UART_Receive_IT(mPtrHndl, inBuf, inBufLen) == HAL_OK) ;
}
//----------------------------------------------------------
/*!
 * @param inBuf Ссылка на контейнер для принятых данных
 * @return При успешном запуске передачи возвращается true
 */
bool TStmUart::receiveIT (std::vector<uint8_t> &inBuf)
{
	return receiveIT (inBuf.data (), inBuf.size()) ;
}
//----------------------------------------------------------
/*!
 * @return При успешной остановке работы DMA возвращается true
 */
bool TStmUart::stopDMA ()
{
	return (HAL_UART_DMAStop(mPtrHndl) == HAL_OK) ;
}
//----------------------------------------------------------
/*!
 * @param inBuf Указатель на буфер для приёма данных
 * @param inBufLen Длинна буфера для приёма данных
 * @return При успешном запуске передачи возвращается true
 */
bool TStmUart::transmit (uint8_t * inBuf, uint32_t inBufLen)
{
	return (HAL_UART_Transmit (mPtrHndl, inBuf, inBufLen, defUartTimeout) == HAL_OK) ;
}
//----------------------------------------------------------
/*!
 * @param inBuf Ссылка на контейнер для принятых данных
 * @return При успешном запуске передачи возвращается true
 */
bool TStmUart::transmit (std::vector<uint8_t> &inBuf)
{
	return transmit (inBuf.data (), inBuf.size()) ;
}
//----------------------------------------------------------
} /* namespace stm */
