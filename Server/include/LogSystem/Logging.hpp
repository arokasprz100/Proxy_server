/**
*	@file Logging.hpp
*	@brief 
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
	*	
	*	@param tags
	*	@param message
	*/
	virtual void logMessage(const std::string& tags, const std::string& message) = 0;
};

#endif // Logging_hpp