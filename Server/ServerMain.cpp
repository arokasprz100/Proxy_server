#include "ServerOwner.hpp"
#include "LogSystem/LogSystem.hpp"
#include "ArgumentParser/ArgumentParser.hpp"
#include "ServerSettings/SettingsEditor.hpp"

int main(int argc, char** argv)
{
	signal(SIGINT, sigintHandler); 
	signal(SIGPIPE, SIG_IGN);
	InputSettings inputSettings = ArgumentParser::parse(argc, argv);
	LogSystem::initialize(inputSettings.getLogType());
	ServerSettings serverSettings;
	serverSettings.readJson("ServerSettings/http_connection_data_example.json");
	if (inputSettings.getIfShouldRunMenu()) {
		SettingsEditor::displayMenu(serverSettings);
	}
	if (!serverSettings.validateJsonFields()) {
		std::cout << "Invalid server settings file" << std::endl;
	}
	ServerOwner::startServer(8080, inputSettings.getClientConnectionType(), serverSettings);
	return 0;
}