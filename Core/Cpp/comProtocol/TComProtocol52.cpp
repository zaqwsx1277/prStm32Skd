/*
 * TComProtocol52.cpp
 *
 *  Created on: 22 мар. 2021 г.
 *      Author: AAL
 */

#include <TComProtocol52.hpp>
#include "TApplication.hpp"
#include "TProtocol.hpp"
#include "TUnit.hpp"
#include "TState.hpp"

namespace comProtocol {

//-----------------------------------------------------------
/*!
 * @param outBuf Ссылка на контейнер с ответом.
 * \todo Переделать на нормальные классы GPIO
 */
void TComProtocol_52::process (const std::vector<uint8_t> &, std::vector<uint8_t> &outBuf, const typeErr)
{
	uint8_t byteData1  { 0 }, byteData2 { 0 }, byteData3 { 0 } ;

	outBuf.push_back (static_cast <uint8_t> (defCommand::cmd52)) ;

	if (app::application-> mListUnits [common::defPort::portLc0] -> getUnitState() == app::usOpen) byteData1 = byteData1 | 0b00010000 ;
	if (app::application-> mListUnits [common::defPort::portLc1] -> getUnitState() == app::usOpen) byteData1 = byteData1 | 0b00100000 ;
	if (app::application-> mListUnits [common::defPort::portLc2] -> getUnitState() == app::usOpen) byteData1 = byteData1 | 0b01000000 ;
	if (app::application-> mListUnits [common::defPort::portLc3] -> getUnitState() == app::usOpen) byteData1 = byteData1 | 0b10000000 ;
	byteData1 = (byteData1 << 4) | (app::application -> mSysInfo.getAlarm(app::alarmUnit::apDoorAlarm) & 0x0F) ;
	outBuf.push_back (byteData1) ;

	outBuf.push_back (byteData2) ;

	if (app::application-> mListUnits [common::defPort::portDs0] -> getUnitState() == app::usOpen) byteData3 = byteData1 | 0b00010000 ;
	if (app::application-> mListUnits [common::defPort::portDs1] -> getUnitState() == app::usOpen) byteData3 = byteData1 | 0b00100000 ;
	if (app::application-> mListUnits [common::defPort::portDs2] -> getUnitState() == app::usOpen) byteData3 = byteData1 | 0b01000000 ;
	if (app::application-> mListUnits [common::defPort::portDs3] -> getUnitState() == app::usOpen) byteData3 = byteData1 | 0b10000000 ;
	byteData1 = (byteData1 << 4) | (app::application -> mSysInfo.getAlarm(app::alarmUnit::apButtonAlarm) & 0x0F) ;
	outBuf.push_back (byteData3) ;

	outBuf.push_back (0) ; 			// Добавляем пустое значение, в которое потом занесем кол-во отправленных сообщений
	uint32_t id { 0 } ;				// Кол-во добавленных сообщений
	for (; id < app::application -> getQueueTxSize();  ++id) {
		if ((app::defDataSize - outBuf.size()) < 24) break ;
		std::shared_ptr <app::TState> element = app::application -> getQueueTxElement() ;
		switch (element-> getTxState()) {
		  case common::defTxState::stTxReady:
		  case common::defTxState::stTxWait: {
			outBuf.push_back (static_cast <uint8_t> (element -> getState ())) ;
			outBuf.push_back (app::application -> mSysInfo.getAddr()) ;
			outBuf.push_back (element -> getUintId ()) ;
			outBuf.push_back ( 0 ) ;
			common::typeKey key = element -> getKey () ;
			outBuf.push_back (0) ;
			outBuf.push_back ((key >> 24)& 0xFF) ;
			outBuf.push_back ((key >> 16)& 0xFF) ;
			outBuf.push_back ((key >> 8)& 0xFF) ;
			outBuf.push_back (key & 0xFF) ;
			uint16_t time = element -> getTime () ;
			outBuf.push_back ((time >> 8)& 0xFF) ;
			outBuf.push_back (time & 0xFF) ;
			element ->setTxState(common::stTxWait) ;
		  }
		  break;

		  default:
		  break;
		}
	}
	outBuf [app::defCom52NumEventPos] = id ;			// Вписываем реальное кол-во отправленных сообщений
}
//-----------------------------------------------------------
} /* namespace comProtocol */
