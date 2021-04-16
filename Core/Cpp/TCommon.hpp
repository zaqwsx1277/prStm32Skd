/*
 * TCommon.hpp
 *
 *  Created on: Feb 15, 2021
 *      Author: energia
 */

#ifndef INC_TCOMMON_H_
#define INC_TCOMMON_H_

#include <stm32f4xx_hal.h>
#include "adc.h"
#include "usart.h"
#include "spi.h"

#include <stdint.h>
#include <memory>
#include <vector>

namespace common {

/*! \defgroup Перечисления
 * \{
 * */
/// Внутренние номера устройств
enum defPort {
	portLc0 = 0x00,			///< Замок 0
	portLc1 = 0x01,			///< Замок 1
	portLc2 = 0x02,			///< Замок 2
	portLc3 = 0x03,			///< Замок 3
	portDs0 = 0x04,			///< Датчик положения двери 0
	portDs1 = 0x05,			///< Датчик положения двери 1
	portDs2 = 0x06,			///< Датчик положения двери 2
	portDs3 = 0x07,			///< Датчик положения двери 3
	portRd0 = 0x12,			///< Считыватель 0	Номера должны быть последовательными, т.к. на их основе будет вычисляться адрес где хрянятся ключи
	portRd1 = 0x13,			///< Считыватель 1
	portRd2 = 0x14,			///< Считыватель 2
	portRd3 = 0x15,			///< Считыватель 3
	portOpen = 0x16,		///< Концевик крышки блока
	portTemp = 0x17,		///< Датчик температуры
	portRdLed0 = 0x18,		///< Индикатор считывателя 0
	portRdLed1 = 0x19,		///< Индикатор считывателя 1
	portRdLed2 = 0x20,		///< Индикатор считывателя 2
	portRdLed3 = 0x21,		///< Индикатор считывателя 3
	portRdAdc = 0x22, 		///< АЦП для контроля считывателей и температуры
	portDoorAdc = 0x23,		///< АЦП для контроля дверей
	portRs485Pull = 0x24,	///< Порт для подтяжки линии RS485
	internalFlash = 0x25,	///< Внутренняя флаш на чипе AT45DB321D

	numPort = 0x26			///< Количество обрабатываемых портов в устройстве
};

/// Идентификаторы возможных событий возникающие при работе ПО
enum class defState : uint8_t {
	doorOpen = 0x01,							///< Нормальный проход
	doorNotOpen = 0x02,							///< Замок был открыт, но дверь не открывалась
	doorErrKey = 0x03,							///< Попытка открыть дверь неизвестным ключок
	sensorOpen = 0x11,							///< Идентификатор события открытия корпуса
	sensorClose = 0x12,							///< Идентификатор события закрытия корпуса
	flashError = 0x17,							///< Ошибка внутренней flash
	hwError = 0x1E,								///< Ошибка аппаратуры
	maxQueueTX = 0x1F,							///< Размер очереди сообщений превышает 256 байт

	unknown = 0xFF								///< Неизвестной породы зверь
} ;

/// Возможные состояния для сообщений о событиях поставленных в очередь ожидания отправки
enum defTxState {
	stTxUnkown,									///< Хрен его знает, что это такое
	stTxReady,									///< Сообщение ждет отправки
	stTxWait,									///< Сообщение включено в пакет и ждет завершения передачи ответа
	stTxSend,									///< Собщение в процессе передачи. Данное состояние нужно для повторной передачи
	stTxSendOk									///< Ответ полностью передан, т.е считаем что сообщение принято и его можно удалять из очереди
};
/*! \} */

/*!
 * \defgroup Системные константы
 * \{
 * */
typedef uint32_t typeKey ;						///< Тип ключей для считывателей

constexpr uint8_t defVersion = 0x20 ;			///< Версия ПО
constexpr uint8_t defTypeSKD = 0x01 ;			///< Тип устройства СКД

///< \todo перенести все адреса в класс TFlash
constexpr uint16_t defFlashPageSize =0x400 ;	///< Размер страницы. FLASH_PAGE_SIZE для F4 не определён :(
constexpr uint16_t defFlashSys = 999 ;			///< Номер блока FLASH для хранения системной информации
constexpr uint32_t defFlashSysPtr = (FLASH_BASE + defFlashSys * defFlashPageSize) ; ///< Адрес блока FLASH для хранения системной информации
constexpr uint32_t defFlashSysAddress = defFlashSysPtr ;///< Указатель на адрес блока
constexpr uint32_t defFlashSysSN = defFlashSysPtr + 2 ;///< Указатель на Серийный Номер блока

constexpr uint16_t defFlashReader = 998 ; 		///< Номер блока FLASH для ключей хранящихся в Считывателях
constexpr uint16_t defKeyNumber = 250 ;			///< Количество ключей
constexpr uint8_t defKeySize = sizeof (typeKey) ;	///< Размер ключа в байтах

constexpr uint8_t defReaderNumber = 4 ;			///< Количество считывателей
constexpr uint8_t defRoomsNumber = 4 ;			///< Количество помещений
constexpr uint32_t defReaderADCNumber = defReaderNumber + 1 ; ///< Количество каналов для АЦП обслуживающих считыватели
constexpr uint16_t defReaderADCCount = 10 ;		///< диапазон усреднения значений считанных с АЦП
constexpr uint16_t defReaderADCMin = 60 ;		///< Минимальное допустимое значение чтения с АЦП
constexpr uint16_t defReaderADCMax = 620 ;		///< Максимальное допустимое значение чтения с АЦП

constexpr uint32_t defQueueReaderLength = 16 ;  ///< Максимальный размер очереди сообщений от считывателей
constexpr uint32_t defQueueOtherLength = 16 ;   ///< Максимальный размер очереди прочих сообщений
constexpr uint32_t defQueueTxLength = 256 ;   	///< Максимальный размер очереди сообщений для отправки по RS

constexpr ADC_HandleTypeDef* defPtrReaderADC { &hadc1 } ; ///< Указатель на АЦП обрабатывающий данные со считывателя
constexpr UART_HandleTypeDef* defPtrUart { &huart2 } ; ///< Указатель на USART
constexpr SPI_HandleTypeDef* defPrtSpiFlash { &hspi2 } ; ///< Указатель на SPI порт на который подключена чип AT45DB321D


//#define defOpenPort GPIOA						///< Порт для концевика
//constexpr uint16_t defOpenPin = GPIO_PIN_0 ; 	///< PIN для концевика

#define defRxTxPort GPIOA						///< Порт для управления приёмом и передачей по UArt
constexpr uint16_t defRxTxPin = GPIO_PIN_1 ;	///< PIN для управления приёмом и передачей по UArt


/*! \} */

}

//-----------------------------------------------------------------
// То, что не собирается под плюсами. :(



#endif /* INC_TCOMMON_H_ */
