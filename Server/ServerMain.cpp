#include "ServerOwner.hpp"
#include "LogSystem/LogSystem.hpp"
#include <memory>

int main()
{
	signal(SIGINT, sigintHandler); 
	LogSystem::initialize(LogType::CONSOLE);
	ServerOwner::startServer(8080);
	return 0;
}