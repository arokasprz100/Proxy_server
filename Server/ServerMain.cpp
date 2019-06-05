#include "ServerOwner.hpp"
#include "LogSystem/LogSystem.hpp"
#include <memory>
#include "ArgumentParser/ArgumentParser.hpp"

int main(int argc, char** argv)
{
	signal(SIGINT, sigintHandler); 
	InputSettings inputSettings = ArgumentParser::parse(argc, argv);
	LogSystem::initialize(inputSettings.getLogType());
	ServerOwner::startServer(8080, inputSettings.getClientConnectionType());
	return 0;
}