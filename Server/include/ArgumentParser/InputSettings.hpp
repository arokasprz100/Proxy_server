#ifndef InputSettings_hpp
#define InputSettings_hpp

#include "../LogSystem/LogType.hpp"
#include "../ClientConnectionType.hpp"

class InputSettings
{
public:

	InputSettings(const LogType& logType, const ClientConnectionType& clientConnectionType, bool shouldRunMenu)
		: logType(logType), clientConnectionType(clientConnectionType), shouldRunMenu(shouldRunMenu) {}

	const LogType& getLogType() const {
		return logType;
	}

	const ClientConnectionType& getClientConnectionType() const {
		return clientConnectionType;
	}

	bool getIfShouldRunMenu() const {
		return shouldRunMenu;
	}

private:

	LogType logType = LogType::SILENT;
	ClientConnectionType clientConnectionType = ClientConnectionType::UNENCRYPTED;
	bool shouldRunMenu = false;


};

#endif // InputSettings_hpp