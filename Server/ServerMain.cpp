#include "ServerOwner.hpp"
#include "LogSystem/LogSystem.hpp"
#include <memory>
#include "ArgumentParser/ArgumentParser.hpp"

int main(int argc, char** argv)
{
	signal(SIGINT, sigintHandler); 
	LogSystem::initialize(LogType::CONSOLE);
	ArgumentParser::parse(argc, argv);
	ServerOwner::startServer(8080);
	return 0;
}