/**
*	@file SSLDataToClientWriter.hpp
*	@brief 
*/


#ifndef SSLDataToClientWriter_hpp
#define SSLDataToClientWriter_hpp

#include "../LogSystem/ConsoleLog.hpp"

/**
*	@class SSLDataToClientWriter
*/
class SSLDataToClientWriter
{
public:
	/**
	*	
	*	@param client
	*	@see Client
	*	@returns 
	*/
	static int write(Client& client) {
		int operationStatus = SSL_write(client.ssl, 
			client.m_httpResponseFromServer.data(),
			client.m_httpResponseFromServer.size());

		if (operationStatus > 0) {
			client.refreshTimestamp();
			client.m_httpResponseFromServer.erase(client.m_httpResponseFromServer.begin(), 
				client.m_httpResponseFromServer.begin() + operationStatus);
		}

		return operationStatus;
	}
};

#endif // SSLDataToClientWriter_hpp