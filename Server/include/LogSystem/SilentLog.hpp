#ifndef SilentLog_hpp
#define SilentLog_hpp

#include "Logging.hpp"

class SilentLog final : public Logging
{
public:

	void logMessage(const std::string& tags, const std::string& message) {
		message + tags;
	}

};


#endif // SilentLog_hpp