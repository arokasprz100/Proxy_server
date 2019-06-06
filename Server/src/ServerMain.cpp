#include "../include/ServerOwner.hpp"
#include "../include/LogSystem/LogSystem.hpp"
#include "../include/ArgumentParser/ArgumentParser.hpp"
#include "../include/ServerSettings/SettingsEditor.hpp"

int main(int argc, char** argv)
{
	signal(SIGINT, sigintHandler); 
	InputSettings inputSettings = ArgumentParser::parse(argc, argv);
	LogSystem::initialize(inputSettings.getLogType());
	ServerSettings serverSettings;
	serverSettings.readJson("./http_connection_data_example.json");
	if (inputSettings.getIfShouldRunMenu()) {
		SettingsEditor::displayMenu(serverSettings);
	}
	if (!serverSettings.validateJsonFields()) {
		std::cout << "Invalid server settings file" << std::endl;
	}
	ServerOwner::startServer(8080, inputSettings.getClientConnectionType(), serverSettings);
	return 0;
}