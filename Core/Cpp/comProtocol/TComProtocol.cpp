/*
 * TComProtocol.cpp
 *
 *  Created on: Feb 16, 2021
 *      Author: AAL
 */

#include "TComProtocol.hpp"
#include "TComProtocolF0.hpp"
#include "TComProtocolFE.hpp"
#include "TComProtocolA0.hpp"
#include "TComProtocolFD.hpp"
#include "TComProtocol01.hpp"
#include "TComProtocol00.hpp"
#include "TComProtocol1B.hpp"
#include "TComProtocolA1.hpp"
#include "TComProtocolA2.hpp"
#include "TComProtocol52.hpp"
#include "TComProtocolF9.hpp"
#include "TComProtocol05.hpp"
#include "TComProtocol13.hpp"
#include "TComProtocol06.hpp"
#include "TComProtocol07.hpp"
#include "TComProtocol08.hpp"
#include "TComProtocolF3.hpp"
#include "TComProtocol09.hpp"

/// Вся обработка принятых команд и передачи ответа
namespace comProtocol {

//---------------------------------------------------------
/*!
 * @details Создание указателя на наследуемый класс для работы патерна Фабрика
 * @param inCommandId Уникальный идентификатор создаваемого класса
 * @return Указатель на созданный класс. Если команды которую прислали нет, то возвращается nullptr
 * \attention Возвращаемый адрес нужно всегда проверять, т.к. может элементарно прилететь левая команда и возврат пойдёт nullptr
 */
std::shared_ptr<TComProtocol> TComProtocol::create (defCommand inCommandId)
{
	std::shared_ptr<TComProtocol> retVal (nullptr) ;

	switch (inCommandId) {
	  case defCommand::cmdF0 :
		retVal = std::make_shared <TComProtocol_F0> () ;
	  break;

	  case defCommand::cmdFE :
		retVal = std::make_shared <TComProtocol_FE> () ;
	  break ;

	  case defCommand::cmdA0 :
		retVal = std::make_shared <TComProtocol_A0> () ;
	  break ;

	  case defCommand::cmd01:
		retVal = std::make_shared <TComProtocol_01> () ;
	  break ;

	  case defCommand::cmd00:
		retVal = std::make_shared <TComProtocol_00> () ;
	  break;

	  case defCommand::cmdFD:
		retVal = std::make_shared <TComProtocol_FD> () ;
	  break ;

	  case defCommand::cmd52:
		retVal = std::make_shared <TComProtocol_52> () ;
	  break ;

	  case defCommand::cmdF3:
		retVal = std::make_shared <TComProtocol_F3> () ;
	  break ;

	  case defCommand::cmdFA:

	  break ;

	  case defCommand::cmd05:
		retVal = std::make_shared <TComProtocol_05> () ;
	  break ;

	  case defCommand::cmd06:
		retVal = std::make_shared <TComProtocol_06> () ;
	  break ;

	  case defCommand::cmd07:
		retVal = std::make_shared <TComProtocol_07> () ;
	  break ;

	  case defCommand::cmd08:
		retVal = std::make_shared <TComProtocol_08> () ;
	  break ;

	  case defCommand::cmd13:
		retVal = std::make_shared <TComProtocol_13> () ;
	  break ;

	  case defCommand::cmdF9:
		retVal = std::make_shared <TComProtocol_F9> () ;
	  break ;

	  case defCommand::cmd1B:
		retVal = std::make_shared <TComProtocol_1B> () ;
	  break ;

	  case defCommand::cmdA1:
		retVal = std::make_shared <TComProtocol_A1> () ;
	  break ;

	  case defCommand::cmdA2:
		retVal = std::make_shared <TComProtocol_A2> () ;
	  break ;

	  case defCommand::cmd09:
		retVal = std::make_shared <TComProtocol_09> () ;
	  break ;

	  default:
	  break;
	}

	return retVal ;
}
//---------------------------------------------------------
} /* namespace comProtocol */
