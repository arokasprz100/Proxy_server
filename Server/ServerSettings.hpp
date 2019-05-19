#ifndef ServerSettings_hpp
#define ServerSettings_hpp

#include <string>

// TODO: refactor
class ServerSettings
{
public:

	static constexpr unsigned BUFFER_SIZE = 50;

	// TODO: change string to filePath (maybe from C++17)
	static constexpr const char* certificateFilePath = "cert.pem";

	static constexpr const char* privateKeyFilePath = "key.pem";
};

#endif // ServerSettings_hpp