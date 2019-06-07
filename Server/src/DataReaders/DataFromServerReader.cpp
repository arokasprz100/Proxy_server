#include "../../include/DataReaders/DataFromServerReader.hpp"

int DataFromServerReader::read(Client& client) 
{
	char buffor[1000] = {'\0'};
	int operationStatus = recv(client.getServerSocket(), buffor, 999, MSG_NOSIGNAL);

	if (operationStatus > 0) {
		LogSystem::logMessage("Received data from server", "SERVER->PROXY", std::to_string(operationStatus), std::to_string(client.getID()));
		client.refreshTimestamp();
		client.m_httpResponseFromServer.insert(client.m_httpResponseFromServer.end(), 
			buffor, buffor + operationStatus);
	} 

	return operationStatus;
}