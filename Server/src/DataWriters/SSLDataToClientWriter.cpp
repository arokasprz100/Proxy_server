#include "../../include/DataWriters/SSLDataToClientWriter.hpp"

int SSLDataToClientWriter::write(Client& client) 
{
	int operationStatus = SSL_write(client.getSSL(), 
		client.m_httpResponseFromServer.data(),
		client.m_httpResponseFromServer.size());

	if (operationStatus > 0) {
		LogSystem::logMessage("Data written to client", "PROXY->CLIENT", std::to_string(operationStatus), std::to_string(client.getID()));
		client.refreshTimestamp();
		client.m_httpResponseFromServer.erase(client.m_httpResponseFromServer.begin(), 
			client.m_httpResponseFromServer.begin() + operationStatus);
	}

	return operationStatus;
}