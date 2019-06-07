/**
*	@file DataFromClientReader.hpp
*	@brief 
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
	*	
	*	@param client
	*	@see Client
	*	@returns 
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
	*	
	*	@param client
	*	@see Client
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