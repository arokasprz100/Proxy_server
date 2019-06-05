#ifndef ServerSettings_hpp
#define ServerSettings_hpp

#include <string>

// TODO: refactor
class ServerSettings
{
public:

	static constexpr const char* certificateFilePath = "certs/domena.crt";

	static constexpr const char* privateKeyFilePath = "certs/domena.key";

};

#endif // ServerSettings_hpp