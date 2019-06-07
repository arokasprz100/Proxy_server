/**
*	@file DataToClientWriter.hpp
*	@brief This file contains DataToClientWriter class definition that implements writing data to client when the connection is over plain HTTP.
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
	*	This function implements writing data to client when the connection is over plain HTTP.
	*	@param client A reference to a Client object from server's std::vector of Client objects.
	*	@see Client
	*	@returns Value returned by send() call.
	*/
	static int write (Client& client) {

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

};

#endif // DataToClientWriter_hpp