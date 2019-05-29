#ifndef ServerSettings_hpp
#define ServerSettings_hpp

#include <string>

// TODO: refactor
class ServerSettings
{
public:

	static constexpr unsigned BUFFER_SIZE = 5000;

	// TODO: change string to filePath (maybe from C++17)
	static constexpr const char* certificateFilePath = "domena.crt";

	static constexpr const char* privateKeyFilePath = "domena.key";
};

#endif // ServerSettings_hpp