#include "../include/ServerOwner.hpp"
#include "../include/LogSystem/LogSystem.hpp"

Server* ServerOwner::server = nullptr;

void ServerOwner::startServer(int port, ClientConnectionType clientConnectionType, const ServerSettings& serverSettings) 
{
	server = new Server(port, clientConnectionType, serverSettings);
	server->startHandlingClients();
}

void ServerOwner::stopServer() 
{
	std::cout << "Controled shutdown" << std::endl;
	if (server) {
		delete server;
		server = nullptr;
	}
}

void sigintHandler(int sig_num) 
{
	LogSystem::logMessage("Signal : " + std::to_string(sig_num), "CLEAN-UP");
	LogSystem::logMessage("Performing clean-up before exiting.", "CLEAN-UP");
	fflush(stdout);
	ServerOwner::stopServer();
	exit(1);
}