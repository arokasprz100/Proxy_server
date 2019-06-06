#ifndef ServerOwner_hpp
#define ServerOwner_hpp

#include "Server.hpp"
#include "ClientConnectionType.hpp"
#include "ServerSettings/ServerSettings.hpp"
#include <signal.h>

class ServerOwner
{
public:

	static void startServer(int port, ClientConnectionType clientConnectionType, const ServerSettings& serverSettings) {
		server = new Server(port, clientConnectionType, serverSettings);
		server->startHandlingClients();
	}

	static void stopServer() {
		std::cout << "Controled shutdown" << std::endl;
		if (server) {
			delete server;
			server = nullptr;
		}
	}

private:

	static Server* server;

};

void sigintHandler(int sig_num);

#endif // ServerOwner_hpp