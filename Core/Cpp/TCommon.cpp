/*
 * TCommon.cpp
 *
 *  Created on: Feb 15, 2021
 *      Author: energia
 */

#include "stm32f4xx_hal.h"

#include "TCommon.hpp"
#include "TApplication.hpp"
#include "TUnitReader.hpp"
#include "TUnitReaderAdc.hpp"

namespace common {

//std::vector <uint16_t> stUnutReaderAdcBufData __attribute__ ((section(".RAM"))) (5, 0xFFFF) ;

}
//-----------------------------------------------------------------------------
/*!
 * \defgroup Callback Обработчики прерываний
 * @{
 */

/*!
 * \brief Обработчик прерываний GPIO
 * @param GPIO_Pin номер сработавшего GPIN
 *
 * \details
 * GPIO_PIN_1, GPIO_PIN_2 - Cчитыватель 1
 * GPIO_PIN_11, GPIO_PIN_12 - Cчитыватель 2
 * GPIO_PIN_15, GPIO_PIN_10 - Считыватель 3
 * GPIO_PIN_8, GPIO_PIN_9 - Считыватель 4
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin) {
	  case GPIO_PIN_1:
		static_cast <app::TUnitReader *> (app::application -> mListUnits [common::portRd0]) -> mPtrUnit -> get(1) ;
	  break;

	  case GPIO_PIN_2:
		static_cast <app::TUnitReader *> (app::application -> mListUnits [common::portRd0]) -> mPtrUnit -> get(0) ;
	  break;

	  case GPIO_PIN_11:
		static_cast <app::TUnitReader *> (app::application -> mListUnits [common::portRd1]) -> mPtrUnit -> get(1) ;
	  break;

	  case GPIO_PIN_12:
		static_cast <app::TUnitReader *> (app::application -> mListUnits [common::portRd1]) -> mPtrUnit -> get(0) ;
	  break;

	  case GPIO_PIN_6:
		static_cast <app::TUnitReader *> (app::application -> mListUnits [common::portRd2]) -> mPtrUnit -> get(1) ;
	  break;

	  case GPIO_PIN_10:
		static_cast <app::TUnitReader *> (app::application -> mListUnits [common::portRd2]) -> mPtrUnit -> get(0) ;
	  break;

	  case GPIO_PIN_8:
		static_cast <app::TUnitReader *> (app::application -> mListUnits [common::portRd3]) -> mPtrUnit -> get(1) ;
	  break;

	  case GPIO_PIN_9:
		static_cast <app::TUnitReader *> (app::application -> mListUnits [common::portRd3]) -> mPtrUnit -> get(0) ;
	  break;

	  default:
	  break;
	}
}
//-----------------------------------------------------------------------------
/*!
 * @brief Обработчик прерываний от таймера
 * @param htim Сработавший таймер
 *
 * @details
 *
 * TIM7 - базовый таймер на 1 мСек для устранения дребезга контактов. Подсчёта окончания посылки co считывателей
 * TIM8 - высоко приоритетный базовый таймер на 50 мСек для обработки АЦП и определения окончания передачи считывателем.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	switch (app::application -> getState ()) {
	  case app::appStarted:
	  break;

	  default:
		  if(htim->Instance == TIM7) {
			  app::application -> checkBounce () ;		// Устраняем дребезг для всех кнопок

		  }
		  if(htim->Instance == TIM8) {
			  app::application -> startConversation () ;				// Запускаем получение значений со всех АЦП

			  static_cast <app::TUnitReader *> (app::application -> mListUnits [common::portRd0]) -> mPtrUnit -> checkFinish () ;	// Проверяем завершения получения всех бит ключей (пауза больше 250 мСек)
			  static_cast <app::TUnitReader *> (app::application -> mListUnits [common::portRd1]) -> mPtrUnit -> checkFinish () ;
			  static_cast <app::TUnitReader *> (app::application -> mListUnits [common::portRd2]) -> mPtrUnit -> checkFinish () ;
			  static_cast <app::TUnitReader *> (app::application -> mListUnits [common::portRd3]) -> mPtrUnit -> checkFinish () ;
		  }
	  break;
	}
}
//-----------------------------------------------------------------------------
/*!
 * \brief Обработчик прерывания от АЦП
 * @param hadc Указатель на сработавший АЦП
 * \details
 * Все АЦП'шки запускаются по таймеру TIM8
 * На ADC1 висят датчики считывателей и датчик температуры
 * На ADC2 висят датчики дверей
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	switch (app::application -> getState ()) {
	  case app::appStarted:
	  break;

	  default:
		  if(hadc->Instance == ADC1)		// Сработал первый АЦП на котором висят считыватели и температура.
		  {
			  app::application -> mListUnits [common::defPort::portRdAdc] -> setUnitState(app::usReady) ;	// Взводим флаг полученных данных и распихиваем их по считывателям
			  app::application -> mListUnits [common::defPort::portRdAdc] -> process () ;
		  }

		  if(hadc->Instance == ADC2)
		  {

		  }
	  break;
	}
}
//------------------------------------------------------------------------------
/*!
 * \brief Обработчик прерывания при отправке сообщения по USART
 *
 *	\attention Нербходимо помнить, что данное прерывание при передаче буфера срабатывает два раза и при изменении конфигурации приекта каждый раз необходимо подправлять файл stm32f1xx_hal_uart.c
 *
 *	Set the UART DMA Half transfer complete callback
 *		huart->hdmatx->XferHalfCpltCallback = UART_DMATxHalfCplt;
 *
 * @param huart Хендл устройства
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	app::application -> mPtrProtocolRS486 -> startReceiveByte() ;
}
//------------------------------------------------------------------------------
/*!
 * \brief Обработчик прерываний при получении сообщений по USART
 * @param huart Хендл устройства
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	switch (app::application -> getState ()) {
	  case app::appProcess:
		if (app::application -> mPtrProtocolRS486 -> receiveByte()) app::application -> setState(app::appReceiveMessage) ;
	  break ;

	  default:
	  break ;
	}
	app::application -> mPtrProtocolRS486 -> startReceiveByte() ;
}
//------------------------------------------------------------------------------
/*!
 * \brief Обработчик прерываний при ошибках
 * \todo Сделать обработку ошибок приёма/передачи данных
 * @param huart Хендл устройства
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{

}
//------------------------------------------------------------------------------

/*! @} */
