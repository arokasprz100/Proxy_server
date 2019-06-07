/**
*	@file SilentLog.hpp
*	@brief This file contains the definition of derivative class SilentLog that doesnt print log messages.
*/

#ifndef SilentLog_hpp
#define SilentLog_hpp

#include "Logging.hpp"

/**
*	@class SilentLog
*/
class SilentLog final : public Logging
{
public:
	/**
	*	This function implements interface described in the Logging class. Its calls dont produce output to std::out.
	*	@see Logging 
	*	@param tags Variable amount of highlighted values inside brackets '[' and ']'.
	*	@param message Message attached to the associated tags.
	*/
	void logMessage(const std::string& tags, const std::string& message) {
		message + tags;
	}

};


#endif // SilentLog_hpp