/*
 * TSysInfo.hpp
 *
 *  Created on: 11 мар. 2021 г.
 *      Author: AAL
 */

#ifndef CPP_TSYSINFO_HPP_
#define CPP_TSYSINFO_HPP_

#include <unordered_set>
#include <vector>
#include <memory>
#include <array>

#include "TSysInfo.hpp"
#include "TCommon.hpp"
#include "TStmSpi.hpp"

namespace app {

constexpr uint8_t defAdestoID = 0x1F ;				///< Идентификатор производителя чипов
constexpr uint8_t defDeviceID = 0x27 ;				///< Идентификатор чипа
constexpr uint8_t defStatusBit = 0x80 ;				///< Бит статуса для команды 0xD7
constexpr uint16_t defNumPage = 8192 ;				///< Кол-во страниц на чипе
constexpr uint16_t defNumByte = 528 ;				///< Кол-во байт на странице
constexpr uint16_t defKeyNumPage = 10 ;				///< Кол-во блоков для хренения ключей

// !!! Номера страниц важны при очистке флешки и др. функций
constexpr uint16_t defFlashPageKey0 = 0 ; 			///< Начальная страница для ключей считывателя 0
constexpr uint16_t defFlashPageKey1 = defFlashPageKey0 + defKeyNumPage ; ///< Начальная страница для ключей считывателя 1
constexpr uint16_t defFlashPageKey2 = defFlashPageKey1 + defKeyNumPage ; ///< Начальная страница для ключей считывателя 2
constexpr uint16_t defFlashPageKey3 = defFlashPageKey2 + defKeyNumPage ; ///< Начальная страница для ключей считывателя 3
constexpr uint16_t defFlashPageProgramm = defFlashPageKey3 + defKeyNumPage ; ///< Программа управления работой блока (функционал элементов)
constexpr uint16_t defFlashPageSysInfo = defFlashPageProgramm + 1; 		 ///< Страница для системной информации
constexpr uint16_t defFlashPageAlarm = defFlashPageSysInfo + 1 ;		 ///< Страница для записи важных событий
constexpr uint16_t defFlashPageState = defFlashPageAlarm + 1 ;			 ///< Страница для записи архива состояний

constexpr uint16_t defFlashAddrSysAddrUnit = 0 ;	///< Адрес блока
constexpr uint16_t defFlashAddrSysSN = 2 ;			///< Серийный номер
constexpr uint16_t defFlashAddrSysNumKey0 = 4 ;		///< Количество используемых ключей для считывателя 0
constexpr uint16_t defFlashAddrSysNumKey1 = 5 ;		///< Количество используемых ключей для считывателя 1
constexpr uint16_t defFlashAddrSysNumKey2 = 6 ;		///< Количество используемых ключей для считывателя 2
constexpr uint16_t defFlashAddrSysNumKey3 = 7 ;		///< Количество используемых ключей для считывателя 3
constexpr uint16_t defFlashAddrSysNumErr = 8 ;		///< Количество системных ошибок

/*!
 * \brief Список обрабатываемых комад для работы с чипом AT45DB321D
 * \ingroup Перечисления
 * \attention Обратить внимание, что для проверки работоспособности чипа выполняется проверка его ID
 * \todo Переделать хранение системной информации поблочно!!! А это значит, что изначально софт разработан неправильно.
 * \todo Сделать подсчёт CRC записанного блока (контроль, что флешка сдохла)
 */
enum class defFlashCmd : uint8_t {
	cmdD7 = 0xD7,	//!< Получение статуса чипа и ожидание окончания записи
	cmd9F = 0x9F,   //!< Получение ID чипа
	cmd0B = 0x0B, 	///< Чтение данных из памяти
	cmd81 = 0x81, 	///< Постраничная очистка флешки
	cmd85 = 0x85	///< Запись через буфер 2
};

/*!
 * \ingroup Перечисления
 * Список обслуживаемых устроуств важных событий, т.е. номер байта в контейнере.
*/
enum alarmUnit {
	apFlag = 0,			///< Лидирующий байт для записи блока важных событий на флешку. Признак занятости блока
	apReaderAlarm,		///< Взлом считывателя
	apButtonAlarm,		///< Взлом кнопки
	apLockAlarm,		///< Взлом замка
	apDoorAlarm,		///< Взлом двери
	apUnitAlarm,		///< Взлом крышки блока
	apCount				///< Кол-во обслуживаемых устройств
};

/*!
 * \brief Класс для хранения системной информации
 * Вся информация хранится в чипе AT45DB321D (528 байт)
 *
 * **Работа с данными о важных событиях**
 *
 * Для каждого типа устройств (alarmUnit) выделяется 8 бит (на каждый датчик по биту)
 * Их состояние записывается последовательно на флешку, пока не заполнится вся страница.
 * Если страница заполнилась, то она стирается и запись начинается по новой.
 * Признаком начала записи является 0 записанный в начальную ячейку и далее записываются байты данных
 *
 * **Работа с ключами**
 *
 * При запуски ключи читаются из флешки и хранятся в unordered_set
 * Ключи записываются в младшие 3 байта. Старший байт остаётся 0xFF. Когда ключ старается, все байты перезаписывается в 0х00.
 * Это сделано для того, что бы при удалении одного ключа не перезаписывать весь блок.
 *
 * \attention Данная флешка может записать ТОЛЬКО (!!!) с одну страницу, но читать может несколько.
 * \attention Я с какого-то перепуга сделал отдельный тип данных для ключа common::typeKey, но не всегда это использую, т.е. его размерность менять нельзя!!!
 *
 * \todo Сделать отдельный класс для работы с флешкой AT45DB
 * \todo передалать хранение всей системной информации в мозгах и лазить на флешку только при её изменении
 * \todo Работа с ключами сделана через жопу. Их нужно перенести в считыватели как делал изначально.
 * \todo Сделать N-кратное резервирование при записи, что бы при чтении выявлять ошибки и автоматически их исправлять. (хм!!! а надо ли?)
 */
class TSysInfo : public stm::TStmSpi {
private:
	std::array <uint8_t, apCount> mAlarmData ;		///< Конетейнер содержащий данные по важным событиям
	uint8_t mAlarmPos { 0 } ;						///< Номер текущей позиции для записи важной информации

	void writeAlarmData () ;						///< Запись содержимого контейнера важных событий на флешак
	bool readKeys (const uint8_t) ;					///< Чтение ключей для считывателя
	bool readKeys (const uint8_t, std::vector<uint8_t> &) ; ///< Чтение ключей для считывателя

	bool sendCmd (defFlashCmd inCmd, const uint16_t inPage = 0, const uint16_t inAddr = 0) ; ///< Отправка команды в порт

	uint8_t mBufRead [defNumByte] ;					///< Буфер для чтения из чипа
	std::unordered_set<common::typeKey> mKey0 ;		///< Контейнер ключей для считывателя 0
	std::unordered_set<common::typeKey> mKey1 ;		///< Контейнер ключей для считывателя 1
	std::unordered_set<common::typeKey> mKey2 ;		///< Контейнер ключей для считывателя 2
	std::unordered_set<common::typeKey> mKey3 ;		///< Контейнер ключей для считывателя 3

public:
	TSysInfo() ;
	virtual ~TSysInfo() = default ;

	bool checkFlash () ;							///< Проверка состояния чипа AT45DB321D

//	std::shared_ptr<uint32_t> getKeys (const uint8_t) ; ///< Чтение ключей из флешки для считывателя
//	bool setKeys (std::shared_ptr<uint32_t>, const uint8_t) ; ///< Запись ключей на флешку для считывателя

	uint8_t getAddr () ;							///< Чтение адреса блока
	uint16_t getSN () ;								///< Чтение серийного номера

	uint16_t getNumKey0 () { return mKey0.size () ; } ///< Количество ключей для считывателя 0
	uint16_t getNumKey1 () { return mKey1.size () ; } ///< Количество ключей для считывателя 1
	uint16_t getNumKey2 () { return mKey2.size () ; } ///< Количество ключей для считывателя 2
	uint16_t getNumKey3 () { return mKey3.size () ; } ///< Количество ключей для считывателя 3
	uint32_t getNumErr () { return 0 ; } 			///< Чтение кол-ва системных ошибок
	uint8_t getNumFlashErr () { return 0 ; } 		///< Чтение кол-ва ошибок флешака (признак того, что сдохла флешка)
	bool setSysInfo (const uint8_t, const uint16_t, const uint32_t) ; ///< Запись на флешку системной информации
	bool erasePage (const uint16_t) ;				///< Очистка указанной страницы

	void setAlarm (alarmUnit, uint16_t) ;			///< Установить флаг важного события
	bool getAlarm (alarmUnit, uint16_t) ;			///< Получить флаг важного события для конкретного датчика
	uint8_t getAlarm (alarmUnit) ;					///< Получить флаг важных событий для всего устройства
	void clearAlarm () ;							///< Очистить флаги важных событий

	bool writeKey (const uint8_t, const common::typeKey) ;	///< Запись ключа в считыватель
	bool deleteKey (uint8_t, common::typeKey) ;		///< Удаление ключа из считывателя
	bool deleteKey (uint8_t) ;						///< Удаление всех ключей из считывателя
	bool shrinkKey (const uint8_t) ;				///< Очистка от ключей помеченных на удаление
};

} /* namespace app */

#endif /* CPP_TSYSINFO_HPP_ */
