#ifndef DataFromClientReaderChooser_hpp
#define DataFromClientReaderChooser_hpp

#include <functional>
#include <map>

#include "../ClientConnectionType.hpp"
#include "../Client.hpp"

class DataFromClientReaderChooser final
{
public:

	static std::function<int(Client&)> chooseUnencryptedDataFromClientReader(ClientConnectionType clientConnectionType) {
		return UNENCRYPTED_DATA_FROM_CLIENT_READERS_BY_CONNECTION_TYPE[clientConnectionType];
	}

	static std::function<int(Client&)> chooseEncryptedDataFromClientReader(ClientConnectionType clientConnectionType) {
		return ENCRYPTED_DATA_FROM_CLIENT_READERS_BY_CONNECTION_TYPE[clientConnectionType];
	}

private:

	static std::map<ClientConnectionType, std::function<int(Client&)>> UNENCRYPTED_DATA_FROM_CLIENT_READERS_BY_CONNECTION_TYPE;
	static std::map<ClientConnectionType, std::function<int(Client&)>> ENCRYPTED_DATA_FROM_CLIENT_READERS_BY_CONNECTION_TYPE;

};

#endif // DataFromClientReaderChooser_hpp