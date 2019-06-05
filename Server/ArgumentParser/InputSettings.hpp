#ifndef InputSettings_hpp
#define InputSettings_hpp

#include "../LogSystem/LogType.hpp"
#include "../ClientConnectionType.hpp"

class InputSettings
{
public:

	InputSettings(const LogType& logType, const ClientConnectionType& clientConnectionType) {
		this->logType = logType;
		this->clientConnectionType = clientConnectionType;
	}

	const LogType& getLogType() const {
		return logType;
	}

	const ClientConnectionType& getClientConnectionType() const {
		return clientConnectionType;
	}

private:

	LogType logType = LogType::SILENT;
	ClientConnectionType clientConnectionType = ClientConnectionType::UNENCRYPTED;


};

#endif // InputSettings_hpp