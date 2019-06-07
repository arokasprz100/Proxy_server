/**
*	@file SettingsEditor.hpp
*	@brief This file contains the interactive command line settings handler.
*/

#ifndef SettingsEditor_hpp
#define SettingsEditor_hpp

#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

#include "ServerSettings.hpp"

/**
*	@class SettingsEditor
*/
class SettingsEditor final
{
public:
	/**
	*	This function prints menu in the command line and handles user inputs.
	*	@param serverSettings A reference to a ServerSettings object containing information read from the json settings file.
	*	@see ServerSettings
	*/
	static void displayMenu(ServerSettings& serverSettings);

private:

	static int getUserChoice(int lowerBound, int upperBound);

	static bool checkIfStringIsNumber(const std::string& input);

	static int convertStringToInt(const std::string& input);

	static void runSettingsMenu(ServerSettings& serverSettings);

	static void changeMaxNumberOfConnections(ServerSettings& serverSettings);
	
	static void changeTimeoutLimit(ServerSettings& serverSettings);

	static void changeDigitSequence(ServerSettings& serverSettings);

	static std::map<int, void(*)(ServerSettings&)> OPERATIONS_BY_USER_INPUT;

};


#endif // SettingsEditor_hpp