/*
 * TUintReader.hpp
 *
 *  Created on: 11 февр. 2021 г.
 *      Author: AAL
 */

#ifndef SRC_TUNITREADER_HPP_
#define SRC_TUNITREADER_HPP_

#include <vector>
#include <set>
#include <unordered_set>
#include <list>

#include "TUnit.hpp"
#include "TWiegand.hpp"

namespace app {
/*!
 * \brief Класс для работы с ридарами Wiegand
 * \todo Написать свой аллокатор для выделения памяти для контейнера типа list, т.к. там течёт пямять. :(
 */
class TUnitReader: public TUnit {
private:
	std::vector <uint16_t> mAdcList ;		///< Список значений считанных с АЦП. Cо списками работать нельзя, т.к. пямять не освобождается и софт тупо пабает
//	std::set <uint32_t> mKeyList ;		///< Список ключей. По хорошему для скорости поиска нужно исппользовать unordered_set
	std::unordered_set <uint32_t> mKeyList ;	///< Список ключей.

public:
	std::unique_ptr <reader::TWiegand> mPtrUnit { nullptr } ;		///< Указатель на класс обрабатывающий данные со считывателя (Блин!!! так не делают)

	TUnitReader (uint8_t);
	virtual ~TUnitReader() = default ;

	unitState process () ;			///< Обработка состояния датчика
	void setProcess (uint32_t inData = 0) { mProcessData = inData ; } 	///< Установка флага выполнения

	void setAdc (uint16_t) ;		///< Добавление значения считанного с АЦП
	uint16_t getMeanAdc () ;		///< Получение усреднённого значений данных прочитанных с АЦП
};

} /* namespace app */

#endif /* SRC_TUNITREADER_HPP_ */
