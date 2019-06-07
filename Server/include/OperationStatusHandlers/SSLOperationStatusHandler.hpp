/**
*	@file SSLOperationStatusHandler.hpp
*	@brief This file contains the definition of the SSL connection to proxy handler.
*/

#ifndef SSLOperationStatusHandler_hpp
#define SSLOperationStatusHandler_hpp

#include "../Client.hpp"

/**
*	@class SSLOperationStatusHandler
*/
class SSLOperationStatusHandler final
{
public:
	/**
	*	This member function implements handling of client SSL to proxy connection.
	*	@param client Given client object from the Server's std::vector of clients.
	*	@see Client
	*	@param operationStatus Value returned by SSL_read() or SSL_write.
	*	@param clientConnectionPollFD Appropriate iterator to the position of the pollfd struct in the main server class' std::vector on which poll() function is used. 
	*/
	static void handle(Client& client, int operationStatus, std::vector<pollfd>::iterator clientConnectionPollFD);

};

#endif // SSLOperationStatusHandler_hpp