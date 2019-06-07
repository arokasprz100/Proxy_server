/**
*	@file ServerOwner.hpp
*	@brief This file contains the definition of functions responsible for launching, stopping as well as handling of signals in this program.
*/

#ifndef ServerOwner_hpp
#define ServerOwner_hpp

#include "Server.hpp"
#include "ClientConnectionType.hpp"
#include "ServerSettings/ServerSettings.hpp"
#include <signal.h>

/**
*	@class ServerOwner
*/
class ServerOwner
{
public:
	/**
	*	This member function starts the server in a way given by arguments.
	*	@praram port This is the number of a port the server will be listening on.
	*	@param clientConnectionType This value describes the kind of connections the server will be accepting.
	*	@see ClientConnectionType
	*	@param serverSettings This object contains the necessary information read from the json settings file.
	*	@see ServerSettings
	*/
	static void startServer(int port, ClientConnectionType clientConnectionType, const ServerSettings& serverSettings) {
		server = new Server(port, clientConnectionType, serverSettings);
		server->startHandlingClients();
	}

	/**
	*	This member function stops the server and performs memory cleanup.
	*/
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

/**
*	@fn This function implements the handling of signal passed as arguemnt.
*	@param sig_num Numerical value describing the signal.
*/
void sigintHandler(int sig_num);

#endif // ServerOwner_hpp