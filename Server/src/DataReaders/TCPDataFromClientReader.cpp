#include "../../include/DataReaders/TCPDataFromClientReader.hpp"

int TCPDataFromClientReader::readUnencryptedData(Client& client) 
{
	char buffor[1000] = {'\0'};
	int operationStatus = recv(client.getClientSocket(), buffor, 999, MSG_NOSIGNAL);
	if (operationStatus > 0) {
		LogSystem::logMessage("Received data from client", "CLIENT->PROXY", std::to_string(operationStatus), std::to_string(client.getID()));
		client.refreshTimestamp();
		client.addDataFromClient(std::vector<char>(buffor, buffor + operationStatus));
	}

	return operationStatus;
}

int TCPDataFromClientReader::readEncryptedData(Client& client) 
{
	char buffor[1000] = {'\0'};
	int operationStatus = recv(client.getClientSocket(), buffor, 999, MSG_NOSIGNAL);
	if (operationStatus > 0) {
		LogSystem::logMessage("Received data from client", "CLIENT->PROXY", std::to_string(operationStatus), std::to_string(client.getID()));
		client.refreshTimestamp();
		client.m_httpRequestFromClient = std::vector<char>(buffor, buffor + operationStatus);
	}
	
	return operationStatus;
}