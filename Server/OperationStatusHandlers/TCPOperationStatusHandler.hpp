#ifndef TCPOperationStatusHandler_hpp
#define TCPOperationStatusHandler_hpp

#include "../Client.hpp"

class TCPOperationStatusHandler final
{
public:

	static void handle(Client& client, int operationStatus, std::vector<pollfd>::iterator clientConnectionPollFD) {
		(void)client;
		(void)operationStatus;
		clientConnectionPollFD->events = POLLIN | POLLOUT;
	}
};

#endif // TCPOperationStatusHandler_hpp