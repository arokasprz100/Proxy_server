/**
*	@file Logging.hpp
*	@brief This file contains the definition of the base class responsible for command line output.
*/

#ifndef Logging_hpp
#define Logging_hpp

#include <string>

/**
*	@class Logging
*/
class Logging
{
public:
	/**
	*	This purely virtual member function provides interface of different logging types that may be selected.
	*	@see ConsoleLog
	*	@see SilentLog
	*	@param tags Variable amount of highlighted values inside brackets '[' and ']'.
	*	@param message Message attached to the associated tags.
	*/
	virtual void logMessage(const std::string& tags, const std::string& message) = 0;
};

#endif // Logging_hpp