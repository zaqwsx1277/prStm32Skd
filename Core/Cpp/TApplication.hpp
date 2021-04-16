/*

 *  Created on: Feb 15, 2021
 *      Author: AAL
 */

#ifndef SRC_X_H_
#define SRC_X_H_

#include <array>
#include <queue>
#include <deque>
#include <vector>
#include <memory>

#include "TUnit.hpp"
#include "TStmGpioInput.hpp"
#include "TState.hpp"
#include "TProtocol.hpp"
#include "TSysInfo.hpp"

/// Всё, что касается управления приложением
namespace app {

class TApplication ;

/*!
 * \ingroup Перечисления
 * \brief Возможные состояния приложения
*/
enum appState {
	appUnknown,//!< appUnknown
	appStarted,			///< Начальный запуск
	appProcess,			///< Обработка сообщений
	appReceiveMessage	///< Получено сообщение от концентратора
};

extern std::unique_ptr <TApplication> application ;

/*!
 * \brief Класс содержащий данные для обеспечения работы приложения
 * \todo Первое, что переделать!!! Использовать std::exceptions
 * \todo Не сделана запись флагов взлома на флешку
 * \attention Две очереди существуют только из-за того, что работу
 */
class TApplication {
private:
	std::deque <std::shared_ptr <TState>> mQueueReader ;///< Контейнер содержащий события считывателей подготовленные для передачи на концентратор
	std::deque <std::shared_ptr <TState>> mQueueOther ;	///< Контейнер содержащий события кроме считывателей подготовленные для передачи на концентратор
	std::deque <std::shared_ptr <TState>> mQueueTx ;	///< Контейнер содержащий обработанные события для отправки на концентратор

	appState mAppState { appStarted } ;			///< Текущее состояния приложения

	uint32_t mComErr { 0 } ;					///< Общее количество различных ошибок

public:
	std::array<app::TUnit *, common::defPort::numPort> mListUnits ;	///< Массив всех обрабатываемых устройств
	std::unique_ptr <TProtocol> mPtrProtocolRS486 { new TProtocol (common::defPtrUart)} ;		///< Указатель на класс для работы по RS485
	TSysInfo mSysInfo ;							///< Класс для работы с информацией записаной на флешку

	TApplication ();
	virtual ~TApplication() = default ;

	appState getState () { return mAppState ; } 				///< Получаем текущее состояние приложения
	void setState (appState inState) { mAppState = inState ; } ///< Установка режима работы приложения
	uint8_t getQueueReaderSize () { return mQueueReader.size () ; } ///< Получить размер очереди сообщений считывателей
	uint8_t getQueueOtherSize () { return mQueueOther.size () ; }	///< Получить размер очереди прочих сообщений
	uint8_t getQueueTxSize () { return mQueueTx.size() ; }			///< Получить размер очереди прочих сообщений
	std::shared_ptr <TState> getQueueReaderElement () ;			///< Чтение и удаление первого необработанного элемента из очереди сообшений считывателей.
	std::shared_ptr <TState> getQueueOtherElement () ;			///< Чтение и удаление первого необработанного элемента из очереди прочих сообшений.
	std::shared_ptr <TState> getQueueTxElement () ;				///< Чтение элемента из очереди для передачи на концентратор
	void setQueueReaderElement (std::shared_ptr <TState>) ;		///< Добавление элемента в очередь сообщений считывателей
	void setgetQueueOtherElement (std::shared_ptr <TState>) ;	///< Добавление элемента в очередь прочий
	void setQueueTxElement (std::shared_ptr <TState>) ;			///< Добавление элемента в очередь сообщений для отправки концентратор

	void manager () ;							///< Обработчик состояний устройств
	void checkBounce () ;						///< Устранение дребезга контактов
	void startConversation () ;					///< Запуск преобразования всех АЦП

};

}  // namespace app





#endif /* SRC_X_H_ */
