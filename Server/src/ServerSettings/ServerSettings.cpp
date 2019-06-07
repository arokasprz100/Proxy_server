#include "../../include/ServerSettings/ServerSettings.hpp"

ServerSettings::ServerSettings() { 
	setCurrentTreeToDefault(); 
}


ServerSettings::~ServerSettings() {
	deleteCurrentTree();
}


void ServerSettings::deleteCurrentTree() {
	if (currentTree != nullptr) {
		delete currentTree;
		currentTree = nullptr;
	}
}


void ServerSettings::createCurrentTree() {
	if (currentTree != nullptr) deleteCurrentTree();
	currentTree = new ptree();
}


void ServerSettings::setCurrentTreeToDefault() {
	createCurrentTree();
	for (auto const& i: treeDefaultvalues) {
		currentTree->put(i.first, i.second);
	}
}


bool ServerSettings::validateJsonFields() {
	if (currentTree != nullptr) {
		std::string connectionData26Numbers = currentTree->get<std::string>("connectionData26Numbers");
		std::string maxNumberOfConnections = currentTree->get<std::string>("maxNumberOfConnections");
		std::string timeoutLimit = currentTree->get<std::string>("timeoutLimit");
		std::string certificateFilePath = currentTree->get<std::string>("certificateFilePath");
		std::string privateKeyFilePath = currentTree->get<std::string>("privateKeyFilePath");

		connectionData26Numbers.erase(std::remove_if(connectionData26Numbers.begin(), connectionData26Numbers.end(), 
			::isspace), connectionData26Numbers.end());

		try {

			if (connectionData26Numbers.length() < 26 || connectionData26Numbers.length() > 26){
				throw WrongJsonDataException("Wrong number sequence format.");
			}

			for (unsigned i=0; i<connectionData26Numbers.length(); i++) {
				if (isdigit(connectionData26Numbers[i]) == false) {
					throw WrongJsonDataException("Wrong number sequence format.");
				}
			}

			for (unsigned i=0; i<maxNumberOfConnections.length(); i++) {
				if (isdigit(maxNumberOfConnections[i]) == false) {
					throw WrongJsonDataException("Incorrect max number of connections");
				}
			}

			for (unsigned i=0; i<timeoutLimit.length(); i++) {
				if (isdigit(timeoutLimit[i]) == false) {
					throw WrongJsonDataException("Incorrect timeout limit");
				}
			}

			std::fstream certificateFile;
			certificateFile.open(certificateFilePath);
			if (certificateFile.fail()) {
				throw WrongJsonDataException("Given certificate file does not exist");
			}
			certificateFile.close();

			std::fstream privateKeyFile;
			privateKeyFile.open(privateKeyFilePath);
			if (privateKeyFile.fail()) {
				throw WrongJsonDataException("Given private key file does not exist");
			}
			privateKeyFile.close();

		} catch (WrongJsonDataException& e) {
			std::cout << e.what() << std::endl;
			return false;
		}

		setConnectionData26Numbers(connectionData26Numbers);
		setMaxNumberOfConnections(maxNumberOfConnections);
		setTimeoutLimit(timeoutLimit);
		setCertificateFilePath(certificateFilePath);
		setPrivateKeyFilePath(privateKeyFilePath);
	}

	return true;
}


void ServerSettings::readJson(const std::string& jsonPath) {
	createCurrentTree();
	read_json(jsonPath, *currentTree);
	validateJsonFields();
}


void ServerSettings::writeJson(const std::string& jsonPath) {
	if (currentTree == nullptr) createCurrentTree();
	write_json(jsonPath, *currentTree);
}



void ServerSettings::printCurrentTree() {
	write_json(std::cout, *currentTree);
}


std::string ServerSettings::getConnectionData26Numbers() const {
	if (currentTree != nullptr) {
		return currentTree->get<std::string>("connectionData26Numbers");
	}
	else return nullptr;
}


std::string ServerSettings::getCertificateFilePath() const {
	if (currentTree != nullptr) {
		return currentTree->get<std::string>("certificateFilePath");
	}
	else return nullptr;
}


std::string ServerSettings::getPrivateKeyFilePath() const {
	if (currentTree != nullptr) {
		return currentTree->get<std::string>("privateKeyFilePath");
	}
	else return nullptr;
}


unsigned ServerSettings::getMaxNumberOfConnections() const {
	if (currentTree != nullptr) {
		std::string maxNumberOfConnections = currentTree->get<std::string>("maxNumberOfConnections");
		std::istringstream iss(maxNumberOfConnections);
		unsigned result;
		iss >> result;
		return result;
	}
	else return 0;
}


unsigned ServerSettings::getTimeoutLimit() const {
	if (currentTree != nullptr) {
		std::string timeoutLimit = currentTree->get<std::string>("timeoutLimit");
		std::istringstream iss(timeoutLimit);
		unsigned result;
		iss >> result;
		return result;
	}
	else return 0;
}


void ServerSettings::setConnectionData26Numbers(const std::string& connectionData26Numbers) {
	if (currentTree != nullptr) {
		currentTree->put("connectionData26Numbers", connectionData26Numbers);
	}
}


void ServerSettings::setCertificateFilePath(const std::string& certificateFilePath) {
	if (currentTree != nullptr) {
		currentTree->put("certificateFilePath", certificateFilePath);
	}
}


void ServerSettings::setPrivateKeyFilePath(const std::string& privateKeyFilePath) {
	if (currentTree != nullptr) {
		currentTree->put("privateKeyFilePath", privateKeyFilePath);
	}
}


void ServerSettings::setMaxNumberOfConnections(const std::string& maxNumberOfConnections) {
	if (currentTree != nullptr) {
		currentTree->put("maxNumberOfConnections", maxNumberOfConnections);
	}
}


void ServerSettings::setTimeoutLimit(const std::string& timeoutLimit) {
	if (currentTree != nullptr) {
		currentTree->put("timeoutLimit", timeoutLimit);
	}
}