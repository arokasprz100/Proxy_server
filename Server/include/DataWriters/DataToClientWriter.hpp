#ifndef DataToClientWriter_hpp
#define DataToClientWriter_hpp

#include "../LogSystem/LogSystem.hpp"

class DataToClientWriter
{
public:

	static int write (Client& client) {

		int operationStatus = send(client.clientSocket,
			client.m_httpResponseFromServer.data(),
			client.m_httpResponseFromServer.size(), MSG_NOSIGNAL);

		LogSystem::logMessage("Data writen to client", "PROXY->CLIENT", std::to_string(operationStatus), std::to_string(client.getID()));

		if (operationStatus > 0) {
			client.refreshTimestamp();
			client.m_httpResponseFromServer.erase(client.m_httpResponseFromServer.begin(),
				client.m_httpResponseFromServer.begin() + operationStatus);
		}

		return operationStatus;
	}

};

#endif // DataToClientWriter_hpp