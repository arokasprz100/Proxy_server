/**
*	@file SilentLog.hpp
*	@brief
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
	*
	*	@param tags
	*	@param message
	*/
	void logMessage(const std::string& tags, const std::string& message) {
		message + tags;
	}

};


#endif // SilentLog_hpp