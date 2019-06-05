#include "OperationStatusHandlerChooser.hpp"

#include "SSLOperationStatusHandler.hpp"
#include "TCPOperationStatusHandler.hpp"

std::map<ClientConnectionType, std::function<void(Client&, int, std::vector<pollfd>::iterator)>> 
	OperationStatusHandlerChooser::OPERATION_STATUS_HANDLERS_BY_CLIENT_CONNECTION_TYPE = 
{
	{
		ClientConnectionType::ENCRYPTED, 
		[](Client& client, int operationStatus, std::vector<pollfd>::iterator clientConnectionPollFD) { 
			SSLOperationStatusHandler::handle(client, operationStatus, clientConnectionPollFD); 
		} 
	},

	{
		ClientConnectionType::UNENCRYPTED, 
		[](Client& client, int operationStatus, std::vector<pollfd>::iterator clientConnectionPollFD) { 
			TCPOperationStatusHandler::handle(client, operationStatus, clientConnectionPollFD); 
		} 
	}
};