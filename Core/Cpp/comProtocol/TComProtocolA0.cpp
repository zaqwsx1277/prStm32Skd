/*
 * TComProtocolA0.cpp
 *
 *  Created on: 15 мар. 2021 г.
 *      Author: AAL
 */

#include "TComProtocolA0.hpp"
#include "TApplication.hpp"
#include "TSysInfo.hpp"

namespace comProtocol {

//------------------------------------------------
/*!
 * \attention Параметры вообще не используются
 */
void TComProtocol_A0::process (const std::vector<uint8_t> &inBuf, std::vector<uint8_t> &outBuf, const typeErr inType)
{
	bool isDeleted { false } ;						// флаг очистки
	uint16_t numPageStart { 0 }, numPageEnd { 0 } ; // Начальный и конечный номера очищаемых страниц
	uint16_t numPageErase { 0 } ;					// Номер стираемой страницы

	outBuf.push_back (static_cast <uint8_t> (defCommand::cmdA0)) ;
	switch (static_cast <defTypeComA0 > (inBuf.at(app::defDataPos))) {
	  case defTypeComA0::full:
		numPageStart = app::defFlashPageKey0 ;
		numPageEnd = app::defFlashPageKey0 + 16 ;
	  break;

	  case defTypeComA0::key0:
		numPageStart = app::defFlashPageKey0 ;
		numPageEnd = app::defFlashPageKey0 + 4 ;
	  break;

	  case defTypeComA0::key1:
		numPageStart = app::defFlashPageKey1 ;
		numPageEnd = app::defFlashPageKey1 + 4 ;
	  break;

	  case defTypeComA0::key2:
		numPageStart = app::defFlashPageKey2 ;
		numPageEnd = app::defFlashPageKey2 + 4 ;
	  break;

	  case defTypeComA0::key3:
		numPageStart = app::defFlashPageKey3 ;
		numPageEnd = app::defFlashPageKey3 + 4 ;
	  break;

	  case defTypeComA0::sysInfo:
		numPageStart = app::defFlashPageSysInfo ;
		numPageEnd = app::defFlashPageSysInfo + 1 ;
	  break;

	  case defTypeComA0::programm:
		numPageStart = app::defFlashPageProgramm ;
		numPageEnd = app::defFlashPageProgramm + 1 ;
	  break;

	  default:
	  break;
	}

	for (numPageErase = numPageStart; numPageErase < numPageEnd; ++numPageErase) {		// Чистим флешку
		isDeleted = app::application -> mSysInfo.erasePage(numPageErase) ;
		if (isDeleted == false) break ;
	}

	if (isDeleted == true) outBuf.push_back (0) ;		// всё прошло хорошо
	  else outBuf.push_back (1) ;						// всё прошло прохо
}
//------------------------------------------------
} /* namespace comProtocol */
