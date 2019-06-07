/**
*	@file DataFromClientReader.hpp
*	@brief This file contains definition of DataFromClientReader that contains methods implementing reading unencrypted and encrypted data from client.
*/

#ifndef DataFromClientReader_hpp
#define DataFromClientReader_hpp

#include "../Client.hpp"
#include <chrono>

/**
*	@class DataFromClientReader
*/
class DataFromClientReader
{
public:
	/**
	*	This member function implements reading unecrypted data from client.
	*	@param client A reference to a Client object from server's std::vector of Client objects.
	*	@see Client
	*	@returns Value returned by recv() call.
	*/
	static int readUnencryptedData(Client& client) {
		char buffor[1000] = {'\0'};
		int operationStatus = recv(client.getClientSocket(), buffor, 999, MSG_NOSIGNAL);
		if (operationStatus > 0) {
			client.refreshTimestamp();
			client.addDataFromClient(std::vector<char>(buffor, buffor + operationStatus));
		}

		return operationStatus;
	}

	/**
	*	This member function implements reading ecrypted data from client.
	*	@param client A reference to a Client object from server's std::vector of Client objects.
	*	@see Client
	*	@returns Value returned by recv() call.
	*/
	static int readEncryptedData(Client& client) {
		char buffor[1000] = {'\0'};
		int operationStatus = recv(client.getClientSocket(), buffor, 999, MSG_NOSIGNAL);
		if (operationStatus > 0) {
			client.refreshTimestamp();
			client.m_httpRequestFromClient = std::vector<char>(buffor, buffor + operationStatus);
		}
		
		return operationStatus;
	}

};

#endif // DataFromClientReader_hpp