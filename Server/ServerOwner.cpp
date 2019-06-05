#include "ServerOwner.hpp"
#include "LogSystem/LogSystem.hpp"

Server* ServerOwner::server = nullptr;

void sigintHandler(int sig_num) 
{
	LogSystem::logMessage("Signal : " + std::to_string(sig_num), "CLEAN-UP");
	LogSystem::logMessage("Performing clean-up before exiting.", "CLEAN-UP");
	fflush(stdout);
	ServerOwner::stopServer();
	exit(1);
}