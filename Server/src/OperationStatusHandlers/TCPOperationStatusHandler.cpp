#include "../../include/OperationStatusHandlers/TCPOperationStatusHandler.hpp"

void TCPOperationStatusHandler::handle(Client& client, int operationStatus, std::vector<pollfd>::iterator clientConnectionPollFD) 
{
	(void)client;
	(void)operationStatus;
	clientConnectionPollFD->events = POLLIN | POLLOUT;
}