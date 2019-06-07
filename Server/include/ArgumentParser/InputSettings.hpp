/**
*	@file InputSettings.hpp
*	@brief This file contains the definition of InputSettings class which defines what options were provided in command line.
*/

#ifndef InputSettings_hpp
#define InputSettings_hpp

#include "../LogSystem/LogType.hpp"
#include "../ClientConnectionType.hpp"

class InputSettings
{
public:
	/**
	*	Constructor.
	*	@param logType A reference to an enum type of LogType.
	*	@see LogType
	*	@param clientConnectionType A reference to an enum of ClientConnectionType.
	*	@see ClientConnectionType
	*	@param shouldRunMenu Tells whether to enter an interactive mode with the program in the command line.
	*/
	InputSettings(const LogType& logType, const ClientConnectionType& clientConnectionType, bool shouldRunMenu)
		: logType(logType), clientConnectionType(clientConnectionType), shouldRunMenu(shouldRunMenu) {}

	/**
	*	A getter function.
	*	@returns A reference to the enum member of type LogType.
	*	@see LogType
	*/
	const LogType& getLogType() const {
		return logType;
	}

	/**
	*	A getter function.
	*	@returns A reference to the enum member of type ClientConnectionType.
	*	@see ClientConnectionType
	*/
	const ClientConnectionType& getClientConnectionType() const {
		return clientConnectionType;
	}

	/**
	*	A getter function.
	*	@returns A logical value whether to enter interactive command line mode.
	*/
	bool getIfShouldRunMenu() const {
		return shouldRunMenu;
	}

private:
	/**
	*	@var logType 
	*/
	LogType logType = LogType::SILENT;

	/**
	*	@var clientConnectionType 
	*/
	ClientConnectionType clientConnectionType = ClientConnectionType::UNENCRYPTED;
	
	/**
	*	@var shouldRunMenu 
	*/
	bool shouldRunMenu = false;
};

#endif // InputSettings_hpp