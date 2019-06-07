#include "../../include/DataWriters/TCPDataToClientWriter.hpp"

int TCPDataToClientWriter::write (Client& client) 
{
	int operationStatus = send(client.getClientSocket(),
		client.m_httpResponseFromServer.data(),
		client.m_httpResponseFromServer.size(), MSG_NOSIGNAL);

	if (operationStatus > 0) {
		LogSystem::logMessage("Data written to client", "PROXY->CLIENT", std::to_string(operationStatus), std::to_string(client.getID()));
		client.refreshTimestamp();
		client.m_httpResponseFromServer.erase(client.m_httpResponseFromServer.begin(),
			client.m_httpResponseFromServer.begin() + operationStatus);
	}

	return operationStatus;
}