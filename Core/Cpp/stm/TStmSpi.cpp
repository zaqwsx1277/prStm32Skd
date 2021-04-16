/*
 * TStmSpi.cpp
 *
 *  Created on: Mar 10, 2021
 *      Author: AAL
 */

#include <stm32f4xx_hal.h>

#include <TStmSpi.hpp>

namespace stm {

/*!
 * @param inHndl Хендл SPI порта
 */
//--------------------------------------------------------
TStmSpi::TStmSpi(SPI_HandleTypeDef *inHndl) {
	mHndl = inHndl ;
}
//-------------------------------------------------------
/*!
 * @param inTimeout Значение таймаута в милисекундах
 */
void TStmSpi::setTimeout (uint32_t inTimeout)
{
	mTimeout = inTimeout ;
}
//-------------------------------------------------------
/*!
 * @param inData Контейнер с данными
 * @return При успешной передаче возвращается true
 */
bool TStmSpi::transmit (std::vector<uint8_t> &inData)
{
	return transmit(inData.data(), inData.size()) ;
}
//-------------------------------------------------------
/*!
 * @param inData Контейнер с данными
 * @return При успешной чтении возвращается true
 */
bool TStmSpi::read (std::vector<uint8_t> &inData)
{
	return read(inData.data(), inData.size()) ;
}
//-------------------------------------------------------
/*!
 * @param inBuf	Буфер с передаваемыми данными
 * @param inBufLen Размер буфера
 * @return При успешной передаче возвращается true
 */
bool TStmSpi::transmit (uint8_t *inBuf, uint32_t inBufLen)
{
	return (HAL_SPI_Transmit(mHndl, inBuf, inBufLen, mTimeout) == HAL_OK) ;
}
//-------------------------------------------------------
/*!
 * @param inBuf Указатель на буфер дл
 * @param inBufLen
 * @return При успешной передаче возвращается true
 */
bool TStmSpi::read (uint8_t *inBuf, uint32_t inBufLen)
{
	return (HAL_SPI_Receive (mHndl, inBuf, inBufLen, mTimeout) == HAL_OK) ;
}
//-------------------------------------------------------
/*!
 * @return При успешном выполнении команды или если порт уже инициализирован возвращается true
 */
bool TStmSpi::init ()
{
	bool retValue { true } ;
	if (mHndl -> State == HAL_SPI_STATE_RESET)
		if (HAL_SPI_Init(mHndl) != HAL_OK) retValue = false ;
	return retValue ;
}
//-------------------------------------------------------
/*!
 * @return При успешном выполнении команды или если порт уже деинициализирован возвращается true
 */
bool TStmSpi::deinit ()
{
	bool retValue { true } ;
	if (mHndl -> State != HAL_SPI_STATE_RESET)
		if (HAL_SPI_DeInit(mHndl) != HAL_OK) retValue = false ;
	return retValue ;
}
//-------------------------------------------------------
bool TStmSpi::transmit (std::vector<uint8_t>::iterator inBegin, uint32_t inNumByte)
{
	bool retValue { true } ;
	uint8_t * ptrBuf = & (*inBegin) ;
//	std::vector <uint8_t> tempBuf (inNum, 0xFF) ;
//
//	std::copy(inBegin, __last, __result)
//
	transmit(ptrBuf, inNumByte) ;
	return retValue ;
}
//-------------------------------------------------------
bool TStmSpi::read (std::vector<uint8_t>::iterator inBegin, uint32_t inNum)
{
	bool retValue { true } ;

	return retValue ;
}
//-------------------------------------------------------
} /* namespace stm */
