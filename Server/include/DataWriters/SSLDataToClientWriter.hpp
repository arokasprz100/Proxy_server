/**
*	@file SSLDataToClientWriter.hpp
*	@brief This file contains SSLDataToClientWriter class definition that implements writing data to client when the connection is over SSL.
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
	*	This function implements writing data to client when the connection is over SSL.
	*	@param client A reference to a Client object from server's std::vector of Client objects.
	*	@see Client
	*	@returns Value returned by OpenSSL's SSL_write call.
	*/
	static int write(Client& client) {
		int operationStatus = SSL_write(client.getSSL(), 
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