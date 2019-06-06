/**
*	@file DataFromServerReader.hpp
*	@brief 
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
	*	
	*	@param client
	*	@see Client
	*	@returns
	*/
	static int read(Client& client) {
		char buffor[1000] = {'\0'};
		int operationStatus = recv(client.serverSocket, buffor, 999, 0);

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