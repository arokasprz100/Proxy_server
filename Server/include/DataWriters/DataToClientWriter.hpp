/**
*	@file DataToClientWriter.hpp
*	@brief 
*/

#ifndef DataToClientWriter_hpp
#define DataToClientWriter_hpp

#include "../LogSystem/LogSystem.hpp"

/**
*	@class DataToClientWriter
*/
class DataToClientWriter
{
public:
	/**
	*	
	*	@param client
	*	@see Client
	*	@returns 
	*/
	static int write (Client& client) {

		int operationStatus = send(client.getClientSocket(),
			client.m_httpResponseFromServer.data(),
			client.m_httpResponseFromServer.size(), MSG_NOSIGNAL);

		if (operationStatus > 0) {
			LogSystem::logMessage("Data writen to client", "PROXY->CLIENT", std::to_string(operationStatus), std::to_string(client.getID()));
			client.refreshTimestamp();
			client.m_httpResponseFromServer.erase(client.m_httpResponseFromServer.begin(),
				client.m_httpResponseFromServer.begin() + operationStatus);
		}

		return operationStatus;
	}

};

#endif // DataToClientWriter_hpp