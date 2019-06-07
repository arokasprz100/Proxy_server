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
	void logMessage(const std::string& tags, const std::string& message){
		std::cout << tags << " " << message << std::endl;

	}

	/**
	*	This function prints to command line passed HTTP request.
	*	@param request A std::vector<char> object containing the HTTP request.
	*/
	static void logHttpRequest(const std::vector<char>& request) {
		std::cout << "[HTTP REQUEST]" << std::endl;
		for(auto& character : request) {
			std::cout << character;
		}
		std::cout << std::endl;
	}
};

#endif // ConsoleLog_hpp