#include "../../include/DataReaders/SSLDataFromClientReader.hpp"

int SSLDataFromClientReader::readUnencryptedData(Client& client) {
	int operationStatus = 0;
	do {
		char buffor[1000] = {'\0'};
		operationStatus = SSL_read(client.getSSL(), buffor, 999);
		if (operationStatus > 0) {
			LogSystem::logMessage("Received data from client", "CLIENT->PROXY", std::to_string(operationStatus), std::to_string(client.getID()));
			client.refreshTimestamp();
			client.addDataFromClient(std::vector<char>(buffor, buffor + operationStatus));
		}

	} while (SSL_pending(client.getSSL()));

	return operationStatus;
}

int SSLDataFromClientReader::readEncryptedData(Client& client) {
	int operationStatus = 0;
	do {
		char buffor[1000] = {'\0'};
		operationStatus = SSL_read(client.getSSL(), buffor, 999);
		if (operationStatus > 0) {
			LogSystem::logMessage("Received data from client", "CLIENT->PROXY", std::to_string(operationStatus), std::to_string(client.getID()));
			client.refreshTimestamp();
			client.m_httpRequestFromClient.insert(client.m_httpRequestFromClient.end(), buffor, buffor + operationStatus);
		}

	} while (SSL_pending(client.getSSL()));

	return operationStatus;
}