#ifndef SSLDataToClientWriter_hpp
#define SSLDataToClientWriter_hpp

#include "../LogSystem/ConsoleLog.hpp"

class SSLDataToClientWriter
{
public:

	static int writeDataToClient(Client& client) {
		int operationStatus = SSL_write(client.ssl, 
			client.m_httpResponseFromServer.data(),
			client.m_httpResponseFromServer.size());
		//ConsoleLog::logMessage("WRITING TO CLIENT STATUS", std::to_string(operationStatus), client.id);
		if (operationStatus > 0) {
			client.m_httpResponseFromServer.erase(client.m_httpResponseFromServer.begin(), 
				client.m_httpResponseFromServer.begin() + operationStatus);
		} else {
			ERR_print_errors_fp(stderr);
		}
		client.clientConnectionPollFD->events = POLLOUT | POLLIN;
		return operationStatus;
	}
};

#endif // SSLDataToClientWriter_hpp