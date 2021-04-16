/*
 * TProtocol.hpp
 *
 *  Created on: Feb 25, 2021
 *      Author: AAL
 */

#ifndef CPP_TPROTOCOL_HPP_
#define CPP_TPROTOCOL_HPP_

#include <vector>
#include <memory>

#include "TStmUart.hpp"

namespace app {

typedef std::vector <uint8_t> typeBufData ;	///< Тип данных для буферов приёма и передачи данных
typedef std::shared_ptr <typeBufData> typePtrBufData ; ///< Тип для указателя на буфер приёма и передачи данных

constexpr std::vector<uint8_t>::size_type defDataSize = 265 ;	///< Максимальный размер буфера приёма/передачи

constexpr uint8_t defStartBytePos = 0x00 ;	///< Позиция стартового байта
constexpr uint8_t defDstPos = 0x01 ;		///< Позиция байта DST
constexpr uint8_t defSrcPos = 0x02 ;		///< Позиция байта SRC
constexpr uint8_t defVersionPos = 0x03 ;	///< Позиция байта версии протокола взаимодействия
constexpr uint8_t defTypePos = 0x04 ;		///< Позиция команды
constexpr uint8_t defDataPos = 0x05 ;    	///< Позиция начала блока DATA

constexpr uint8_t defCom01SnPos = 0x05 ;	///< Для команды 01 позиция серийного номера
constexpr uint8_t defCom01AdrPos = 0x07 ;	///< Для команды 01 позиция адреса блока

constexpr uint32_t defCom52NumEventPos = 0x09 ;	///< Для команды запроса диагностики (0х52) позиция кол-ва событий

constexpr uint8_t defStartByte = 0x1F ;		///< Стартовый байт сообщения
constexpr uint8_t defVersion = 0x24 ;		///< Версия протокола взаимодействия
constexpr uint8_t defEndByte = 0x2F ;		///< Признак конца сообщения
constexpr uint8_t defStopByte = 0x55 ; 		///< Стоповый байт сообщения

/*!
 * \brief Класс обработки протокола взаимодействия по RS485
 */
class TProtocol : public stm::TStmUart  {
private:
	uint8_t mByte { 0 }, mPrevByte { 0 } ; ///< Текущий и предыдущий байт принятые по RS
	std::vector<uint8_t> mDataRx ;		///< Буфер для принятого сообщения
	std::vector<uint8_t> mDataTx ;		///< Буфер для передаваемого сообщения
public:
	TProtocol(UART_HandleTypeDef*);
	virtual ~TProtocol() = default ;

	uint16_t getCRC (std::vector<uint8_t>&) ; ///< Формирование CRC
	uint16_t setRxCRC () ; 		///< Формирование CRC по буферу mDataRx
	bool receiveByte () ; 		///< Обработка принятого байта
	bool receiveByte (uint8_t) ; ///< Обработка принятого байта
	bool startReceiveByte () ;  ///< Запуск ожидания получения данных
	bool makeAnswer () ;		///< На основании полученной команды формируем ответ и отправляем его на концентратор
	void checkControlByte (uint8_t) ; ///< Проверка добавляемого байта и при необходимости его дублирование
};

} /* namespace app */

#endif /* CPP_TPROTOCOL_HPP_ */
