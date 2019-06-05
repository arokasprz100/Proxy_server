#include "SettingsEditor.hpp"

std::map<int, void(*)(ServerSettings&)> SettingsEditor::OPERATIONS_BY_USER_INPUT = 
{
	{1, &SettingsEditor::changeMaxNumberOfConnections},
	{2, &SettingsEditor::changeTimeoutLimit},
	{3, &SettingsEditor::changeDigitSequence}
};


void SettingsEditor::displayMenu(ServerSettings& serverSettings) 
{
	while(true) 
	{
		std::cout << "Choose action\n" << "1. Change server settings\n"
			<< "2. Run server\n" << "3. Exit" << std::endl;

		int userInput = getUserChoice(1, 3);

		if (userInput == 1) {
			runSettingsMenu(serverSettings);
		}
		else if (userInput == 2) {
			return;
		}
		else {
			exit(1);
		}
	}
}


int SettingsEditor::getUserChoice(int lowerBound, int upperBound) 
{
	while(true) 
	{
		std::string userInput;
		std::cin >> userInput;
		if (checkIfStringIsNumber(userInput)) {
			int choice = convertStringToInt(userInput);
			if (choice >= lowerBound && choice <= upperBound) {
				return choice;
			}
		}
		std::cout << "Wrong input. Try again!" << std::endl;
	}
}


bool SettingsEditor::checkIfStringIsNumber(const std::string& input)
{
	return !input.empty() && 
		std::find_if(input.begin(), input.end(), [](char c) { 
			return !std::isdigit(c); 
		}) == input.end();
}


int SettingsEditor::convertStringToInt(const std::string& input)
{
	int number;
	std::istringstream iss (input);
	iss >> number;
	return number;
}


void SettingsEditor::runSettingsMenu(ServerSettings& serverSettings) 
{
	while(true) 
	{
		std::cout << "Current settings: " << std::endl;
		serverSettings.printCurrentTree();

		std::cout << "Choose action\n" 
			<< "1. Change max number of connections\n"
			<< "2. Change connection timeout\n" 
			<< "3. Change 26-digit sequence\n" 
			<< "4. Apply and return\n" 
			<< "5. Apply, save and return" << std::endl;

		int userInput = getUserChoice(1, 5);

		if (userInput == 4) {
			if (serverSettings.validateJsonFields()) {
				return;
			}
		}
		else if (userInput == 5) {
			if (serverSettings.validateJsonFields()) {
				serverSettings.writeJson("ServerSettings/http_connection_data_example.json");
				return;
			}
		}
		else {
			OPERATIONS_BY_USER_INPUT[userInput](serverSettings);
		}
	}
}


void SettingsEditor::changeMaxNumberOfConnections(ServerSettings& serverSettings) {
	std::cout << "Please enter new number of connections [1 - 1000]" << std::endl;
	int maxNumberOfConnections = getUserChoice(1, 1000);
	serverSettings.setMaxNumberOfConnections(std::to_string(maxNumberOfConnections));
}


void SettingsEditor::changeTimeoutLimit(ServerSettings& serverSettings) {
	std::cout << "Please enter new connection timeout [1 - 3600]" << std::endl;
	int connectionTimeout = getUserChoice(1, 3600);
	serverSettings.setTimeoutLimit(std::to_string(connectionTimeout));
}


void SettingsEditor::changeDigitSequence(ServerSettings& serverSettings) {
	std::cout << "Please enter new 26-digit sequence" << std::endl;
	std::string sequence;
	std::cin.ignore();
	std::getline(std::cin, sequence);
	serverSettings.setConnectionData26Numbers(sequence);
}