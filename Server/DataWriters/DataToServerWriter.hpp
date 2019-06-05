#ifndef DataToServerWriter_hpp
#define DataToServerWriter_hpp

#include "../LogSystem/LogSystem.hpp"

class DataToServerWriter
{
public:

	static int write(Client& client) {

		int operationStatus = send(client.serverSocket, 
				client.m_httpRequestFromClient.data(), 
				client.m_httpRequestFromClient.size(), MSG_NOSIGNAL);

		LogSystem::logMessage("Data writen to server", "PROXT->SERVER", std::to_string(operationStatus), std::to_string(client.getID()));
		
		if (operationStatus > 0) {
			client.refreshTimestamp();
			client.m_httpRequestFromClient.erase(client.m_httpRequestFromClient.begin(), 
				client.m_httpRequestFromClient.begin() + operationStatus);
		}
		
		return operationStatus;
	}
};

#endif // DataToServerWriter_hpp