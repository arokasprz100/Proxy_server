#include "../../include/OperationStatusHandlers/SSLOperationStatusHandler.hpp"

void SSLOperationStatusHandler::handle(Client& client, int operationStatus, std::vector<pollfd>::iterator clientConnectionPollFD) 
{
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