#ifndef DataFromClientReader_hpp
#define DataFromClientReader_hpp

#include "../Client.hpp"
#include <chrono>

class DataFromClientReader
{
public:

	static int readUnencryptedData(Client& client) {
		char buffor[1000] = {'\0'};
		int operationStatus = recv(client.clientSocket, buffor, 999, MSG_NOSIGNAL);
		if (operationStatus > 0) {
			client.refreshTimestamp();
			client.addDataFromClient(std::vector<char>(buffor, buffor + operationStatus));
		}

		return operationStatus;
	}

	static int readEncryptedData(Client& client) {
		char buffor[1000] = {'\0'};
		int operationStatus = recv(client.clientSocket, buffor, 999, MSG_NOSIGNAL);
		if (operationStatus > 0) {
			client.refreshTimestamp();
			client.m_httpRequestFromClient = std::vector<char>(buffor, buffor + operationStatus);
		}
		
		return operationStatus;
	}

};

#endif // DataFromClientReader_hpp