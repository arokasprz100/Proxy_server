/**
*	@file SSLOperationStatusHandler.hpp
*	@brief
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
	*
	*	@param client
	*	@see Client
	*	@param operationStatus
	*	@param clientConnectionPollFD
	*/
	static void handle(Client& client, int operationStatus, std::vector<pollfd>::iterator clientConnectionPollFD) {
		if (operationStatus != 0) {
			int errors = SSL_get_error(client.getSSL(), operationStatus);
			if (errors == SSL_ERROR_WANT_READ) {
				clientConnectionPollFD->events |= POLLIN;
			}
			else if (errors == SSL_ERROR_WANT_WRITE) {
				clientConnectionPollFD->events |= POLLOUT;
			}
		}
	}

};

#endif // SSLOperationStatusHandler_hpp