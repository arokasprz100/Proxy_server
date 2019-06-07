/**
*	@file DataFromClientReaderChooser.hpp
*	@brief This file contains the definition of DataFromClientReaderChooser that handles assigning handlers for appropriate connection types.
*/

#ifndef DataFromClientReaderChooser_hpp
#define DataFromClientReaderChooser_hpp

#include <functional>
#include <map>

#include "../ClientConnectionType.hpp"
#include "../Client.hpp"

/**
*	@class DataFromClientReaderChooser
*/
class DataFromClientReaderChooser final
{
public:
	/**
	*	This member function returns appropriate UnencryptedDataFromClientReader function.
	*	@param clientConnectionType Describes the type of client to destination server connection.
	*	@see ClientConnectionType
	*	@returns Appropriate function to handle unencrypted data reading from client.
	*/
	static std::function<int(Client&)> chooseUnencryptedDataFromClientReader(ClientConnectionType clientConnectionType) {
		return UNENCRYPTED_DATA_FROM_CLIENT_READERS_BY_CONNECTION_TYPE[clientConnectionType];
	}

	/**
	*	This member function returns appropriate EncryptedDataFromClientReader function.
	*	@param clientConnectionType Describes the type of client to destination server connection.
	*	@see ClientConnectionType
	*	@returns Appropriate function to handle encrypted data reading from client.
	*/
	static std::function<int(Client&)> chooseEncryptedDataFromClientReader(ClientConnectionType clientConnectionType) {
		return ENCRYPTED_DATA_FROM_CLIENT_READERS_BY_CONNECTION_TYPE[clientConnectionType];
	}

private:

	static std::map<ClientConnectionType, std::function<int(Client&)>> UNENCRYPTED_DATA_FROM_CLIENT_READERS_BY_CONNECTION_TYPE;
	static std::map<ClientConnectionType, std::function<int(Client&)>> ENCRYPTED_DATA_FROM_CLIENT_READERS_BY_CONNECTION_TYPE;

};

#endif // DataFromClientReaderChooser_hpp