/**
*	@file DataFromServerReader.hpp
*	@brief This file contains definition of DataFromServerReader implementing reading data received from destination server.
*/

#ifndef DataFromServerReader_hpp
#define DataFromServerReader_hpp

#include "../LogSystem/LogSystem.hpp"

/**
*	@class DataFromServerReader
*/
class DataFromServerReader
{
public:
	/**
	*	This member function implements reading data from destination server.
	*	@param client A reference to a Client object from server's std::vector of Client objects.
	*	@see Client
	*	@returns Value returned by recv() call.
	*/
	static int read(Client& client) {
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
};

#endif // DataFromServerReader_hpp