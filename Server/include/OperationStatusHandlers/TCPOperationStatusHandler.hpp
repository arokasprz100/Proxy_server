/**
*	@file TCPOperationStatusHandler.hpp
*	@brief This file contains the definition of the plain HTTP connection to proxy handler.
*/

#ifndef TCPOperationStatusHandler_hpp
#define TCPOperationStatusHandler_hpp

#include "../Client.hpp"

/**
*	@class TCPOperationStatusHandler
*/
class TCPOperationStatusHandler final
{
public:
	/**
	*	This member function implements handling of client plain HTTP to proxy connection.
	*	@param client Given client object from the Server's std::vector of clients.
	*	@see Client
	*	@param operationStatus Value returned by either send() or recv() call.
	*	@param clientConnectionPollFD Appropriate iterator to the position of the pollfd struct in the main server class' std::vector on which poll() function is used. 
	*/
	static void handle(Client& client, int operationStatus, std::vector<pollfd>::iterator clientConnectionPollFD) {
		(void)client;
		(void)operationStatus;
		clientConnectionPollFD->events = POLLIN | POLLOUT;
	}
};

#endif // TCPOperationStatusHandler_hpp