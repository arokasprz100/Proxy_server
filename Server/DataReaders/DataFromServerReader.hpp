#ifndef DataFromServerReader_hpp
#define DataFromServerReader_hpp

#include "../LogSystem/LogSystem.hpp"

class DataFromServerReader
{
public:

	static int readDataFromServer(Client& client) {
		char buffor[1009] = {'\0'};
		int operationStatus = recv(client.serverSocket, buffor, 999, 0);
		//ConsoleLog::logMessage("RECV FROM SERVER STATUS", std::to_string(operationStatus), client.id);
		if (operationStatus > 0) {
			client.m_httpResponseFromServer.insert(client.m_httpResponseFromServer.end(), 
				buffor, 
				buffor + operationStatus);
		} 
		client.serverConnectionPollFD->events = POLLOUT | POLLIN;
		client.clientConnectionPollFD->events = POLLOUT | POLLIN;
		return operationStatus;
	}
};

#endif // DataFromServerReader_hpp