/*
 * TStmSpi.hpp
 *
 *  Created on: Mar 10, 2021
 *      Author: energia
 */

#ifndef CPP_STM_TSTMSPI_HPP_
#define CPP_STM_TSTMSPI_HPP_

#include <spi.h>

#include <vector>
#include <array>

namespace stm {

constexpr uint32_t defSpiTimeout = 100 ;		///< Значение таймаута по умолчанию

/// Класс обслуживающий SPI
/*!
 * \brief Класс обслуживающий порт SPI
 * \todo !!! убрать костыль с GPIO
 */
class TStmSpi {
private:
	uint32_t mTimeout { defSpiTimeout } ;		///< Тайаут для устройства
	SPI_HandleTypeDef *mHndl ;					///< Хендл устройства

public:
	TStmSpi (SPI_HandleTypeDef *);
	virtual ~TStmSpi() = default ;

	void setTimeout (uint32_t) ;				///< Установка таймаута

	bool init () ;								///< Инициализация порта
	bool deinit () ;							///< деинициализация порта

	bool transmit (std::vector<uint8_t> &) ;	///< передача данных на устройство
	bool read (std::vector<uint8_t> &) ;		///< чтение данных с устройства
	bool transmit (uint8_t *, uint32_t) ;		///< передача данных на устройство
	bool read (uint8_t *, uint32_t) ;			///< чтение данных с устройства
	bool transmit (std::vector<uint8_t>::iterator, uint32_t) ; ///< передача данных на устройство
	bool read (std::vector<uint8_t>::iterator, uint32_t) ;	   ///< чтение данных с устройства
};

} /* namespace stm */

#endif /* CPP_STM_TSTMSPI_HPP_ */
