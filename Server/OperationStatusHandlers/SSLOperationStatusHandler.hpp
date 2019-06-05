#ifndef SSLOperationStatusHandler_hpp
#define SSLOperationStatusHandler_hpp

#include "../Client.hpp"

class SSLOperationStatusHandler final
{
public:

	static void handle(Client& client, int operationStatus, std::vector<pollfd>::iterator clientConnectionPollFD) {
		if (operationStatus != 0) {
			int errors = SSL_get_error(client.ssl, operationStatus);
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