/*
 * TComProtocol.hpp
 *
 *  Created on: Feb 16, 2021
 *      Author: energia
 */

#ifndef CPP_TCOMPROTOCOL_HPP_
#define CPP_TCOMPROTOCOL_HPP_

#include <memory>
#include "TProtocol.hpp"

namespace comProtocol {

/*!
 * \ingroup Перечисления
 * \brief Список возможных команд
 * \attention Команда А0 работает без подтверждения и это ядерная мина замедленного действия!!!
 */
enum class defCommand : uint8_t {
	cmdUnknown = 0xFF, ///< Неизвестная команда
	cmdF0 = 0xF0, 	//!< Обработка ошибки
	cmdFE = 0xFE, 	//!< Проверка адреса и заводского номера устройства
	cmd01 = 0x01, 	//!< Команда установки адреса блока и заводского номера
	cmd00 = 0x00, 	//!< Команда установки заводского номера
	cmdFD = 0xFD, 	//!< Программирование RTC
	cmd1B = 0x1B, 	//!< Программный перезапуск контроллера
	cmd52 = 0x52, 	//!< Запрос диагностики
	cmdA2 = 0xA2, 	///< Запрос сервисной информации
	cmdFA = 0xFA, 	//!<
	cmd05 = 0x05, 	//!< Открыть указанную или все двери
	cmd06 = 0x06, 	//!< Закрыть указанную или все двери
	cmd07 = 0x07, 	//!< Заблокировать указанную или все двери
	cmd08 = 0x08, 	//!< Разблокировать указанную или все двери
	cmd09 = 0x09, 	///< Записать в считыватель новый ключ
	cmd0C = 0x0C,	///< Удалить из считывателя ключ
	cmdF3 = 0xF3, 	//!< Удаление всех ключей из считывателя
	cmd13 = 0x13, 	//!< Открыть все двери
	cmdF9 = 0xF9, 	//!< Подбор длинны линии канала RS485
	cmdA0 = 0xA0, 	///< Сброс контроллера в заводские установки
	cmdA1 = 0xA1	///< Зарезервированная команда для отправки на концентратор ответа об успешном программном (команда 1B) перезапуске контроллера
};

/*!
 * \ingroup Перечисления
 * \brief Причины возможных ошибок
 */
enum class typeErr : uint8_t {
	noError = 0,	//!< Ошибка отсутствует
	errCRC = 1,     //!< Ошибка контрольной суммы
	errType = 2,    //!< Ошибка в типе команды
	errVersion = 3, //!< Ошибка версии протокола
	errWriteSN = 4, //!< Ошибка при записи Серийного Номера
	errWriteAdr = 5,//!< Ошибка при записи Адреса блока
	errFormat = 6   //!< Ошибка в формате сообщения
};
/*!
 * \brief Базовый класс для патерна типа Фабрика для обмена данными между устройствами
 */
class TComProtocol {
public:
	TComProtocol() = default;
	virtual ~TComProtocol() = default ;

	static std::shared_ptr<TComProtocol> create (defCommand) ;	///< Создание указателя на наследуемый класс
	virtual void process (const std::vector<uint8_t> &inBuf, std::vector<uint8_t> &outBuf,  const typeErr inErr = typeErr::noError) = 0 ;	///< Обработка полученной команды
};

} /* namespace comProtocol */

#endif /* CPP_TCOMPROTOCOL_HPP_ */
