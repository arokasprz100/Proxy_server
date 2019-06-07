/**
*	@file DataToServerWriter.hpp
*	@brief This file contains DataToServerWriter class definition that implements writing data to destination server.
*/

#ifndef DataToServerWriter_hpp
#define DataToServerWriter_hpp

#include "../LogSystem/LogSystem.hpp"

/**
*	@class DataToServerWriter
*/
class DataToServerWriter
{
public:
	/**
	*	This member function implements writing data to destination server.
	*	@param client A reference to a Client object from server's std::vector of Client objects.
	*	@see Client
	*	@returns Value returned by send() call.
	*/
	static int write(Client& client) {

		int operationStatus = send(client.getServerSocket(), 
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