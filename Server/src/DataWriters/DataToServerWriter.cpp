#include "../../include/DataWriters/DataToServerWriter.hpp"

int DataToServerWriter::write(Client& client)
{
	int operationStatus = send(client.getServerSocket(), 
			client.m_httpRequestFromClient.data(), 
			client.m_httpRequestFromClient.size(), MSG_NOSIGNAL);
	
	if (operationStatus > 0) {
		LogSystem::logMessage("Data written to server", "PROXY->SERVER", std::to_string(operationStatus), std::to_string(client.getID()));
		client.refreshTimestamp();
		client.m_httpRequestFromClient.erase(client.m_httpRequestFromClient.begin(), 
			client.m_httpRequestFromClient.begin() + operationStatus);
	}
	
	return operationStatus;
}