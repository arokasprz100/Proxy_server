/**
*	@file ConsoleLog.hpp
*	@brief This file contains the definition of derivative class ConsoleLog that prints log messages to std::out.
*/

#ifndef ConsoleLog_hpp
#define ConsoleLog_hpp

#include <iostream>
#include <string>
#include <vector>

#include "Logging.hpp"

/**
*	@class ConsoleLog
*/
class ConsoleLog final : public Logging
{
public:
	/**
	*	This function implements interface described in the Logging class.
	*	@see Logging
	*	@param tags Variable amount of highlighted values inside brackets '[' and ']'.
	*	@param message Message attached to the associated tags.
	*/
	void logMessage(const std::string& tags, const std::string& message) override {
		std::cout << tags << " " << message << std::endl;
	}

};

#endif // ConsoleLog_hpp