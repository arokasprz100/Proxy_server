/**
*	@file TCPOperationStatusHandler.hpp
*	@brief
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
	*
	*	@param client
	*	@see Client
	*	@param operationStatus
	*	@param clientConnectionPollFD
	*/
	static void handle(Client& client, int operationStatus, std::vector<pollfd>::iterator clientConnectionPollFD) {
		(void)client;
		(void)operationStatus;
		clientConnectionPollFD->events = POLLIN | POLLOUT;
	}
};

#endif // TCPOperationStatusHandler_hpp