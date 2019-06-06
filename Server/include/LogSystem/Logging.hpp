#ifndef Logging_hpp
#define Logging_hpp

#include <string>

class Logging
{
public:

	virtual void logMessage(const std::string& tags, const std::string& message) = 0;
};

#endif // Logging_hpp