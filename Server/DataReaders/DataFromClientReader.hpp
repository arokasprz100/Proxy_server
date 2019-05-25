#ifndef DataFromClientReader_hpp
#define DataFromClientReader_hpp

#include "../Client.hpp"

class DataFromClientReader
{
public:

	static int readUnencryptedDataFromClient(Client& client) {
		char buffor[3000] = {'\0'};
		int operationStatus = recv(client.clientSocket, buffor, 2999, 0);
		//std::cout << "[RECV FROM CLIENT STATUS] " << operationStatus << std::endl;
		if (operationStatus > 0) {
			client.addDataFromClient(std::vector<char>(buffor, buffor + operationStatus));
			client.clientConnectionPollFD->events = POLLIN;
		} 
		else {
			client.clientConnectionPollFD->events = POLLIN;
		}
		return operationStatus;
	}

	static int readEncryptedDataFromClient(Client& client) {
		char buffor[30000] = {'\0'};
		//operationStatus = SSL_read(client.ssl, buffor, 999);
		int operationStatus = recv(client.clientSocket, buffor, 29999, 0);
		if (operationStatus > 0) {
			//std::cout << "[RECV FROM CLIENT STATUS] " << operationStatus << std::endl;
			client.m_httpRequestFromClient = std::vector<char>(buffor, buffor + operationStatus);
			client.clientConnectionPollFD->events = POLLOUT;
		}
		else {
			client.clientConnectionPollFD->events = POLLIN;
		}
		return operationStatus;
	}

};

#endif // DataFromClientReader_hpp