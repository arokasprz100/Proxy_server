#ifndef ServerSettings_hpp
#define ServerSettings_hpp

#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;
class ServerSettings final 
{
public:

	ServerSettings() { 
		setCurrentTreeToDefault(); 
	}

	~ServerSettings() {
		deleteCurrentTree();
	}

	void deleteCurrentTree() {
		if (currentTree != nullptr) {
			delete currentTree;
			currentTree = nullptr;
		}
	}

	void createCurrentTree() {
		if (currentTree != nullptr) deleteCurrentTree();
		currentTree = new ptree();
	}

	void setCurrentTreeToDefault() {
		createCurrentTree();
		for (auto const& i: treeDefaultvalues) {
			currentTree->put(i.first, i.second);
		}
	}

	bool validateJsonFields() {
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

	void readJson(std::string jsonPath) {
		createCurrentTree();
		read_json(jsonPath, *currentTree);
		validateJsonFields();
	}

	void writeJson(const std::string& jsonPath) {
		if (currentTree == nullptr) createCurrentTree();
		write_json(jsonPath, *currentTree);
	}


	// Debug function
	void printCurrentTree() {
		write_json(std::cout, *currentTree);
	}


	std::string getConnectionData26Numbers() const {
		if (currentTree != nullptr) {
			return currentTree->get<std::string>("connectionData26Numbers");
		}
		else return nullptr;
	}

	std::string getCertificateFilePath() const {
		if (currentTree != nullptr) {
			return currentTree->get<std::string>("certificateFilePath");
		}
		else return nullptr;
	}

	std::string getPrivateKeyFilePath() const {
		if (currentTree != nullptr) {
			return currentTree->get<std::string>("privateKeyFilePath");
		}
		else return nullptr;
	}

	unsigned getMaxNumberOfConnections() const {
		if (currentTree != nullptr) {
			std::string maxNumberOfConnections = currentTree->get<std::string>("maxNumberOfConnections");
			std::istringstream iss(maxNumberOfConnections);
			unsigned result;
			iss >> result;
			return result;
		}
		else return 0;
	}

	unsigned getTimeoutLimit() const {
		if (currentTree != nullptr) {
			std::string timeoutLimit = currentTree->get<std::string>("timeoutLimit");
			std::istringstream iss(timeoutLimit);
			unsigned result;
			iss >> result;
			return result;
		}
		else return 0;
	}

	// Setters for internal server use in order to modify
	// json values accordingly.
	void setConnectionData26Numbers(const std::string& connectionData26Numbers) {
		if (currentTree != nullptr) {
			currentTree->put("connectionData26Numbers", connectionData26Numbers);
		}
	}

	void setCertificateFilePath(const std::string& certificateFilePath) {
		if (currentTree != nullptr) {
			currentTree->put("certificateFilePath", certificateFilePath);
		}
	}

	void setPrivateKeyFilePath(const std::string& privateKeyFilePath) {
		if (currentTree != nullptr) {
			currentTree->put("privateKeyFilePath", privateKeyFilePath);
		}
	}

	void setMaxNumberOfConnections(const std::string& maxNumberOfConnections) {
		if (currentTree != nullptr) {
			currentTree->put("maxNumberOfConnections", maxNumberOfConnections);
		}
	}

	void setTimeoutLimit(const std::string& timeoutLimit) {
		if (currentTree != nullptr) {
			currentTree->put("timeoutLimit", timeoutLimit);
		}
	}


private:
	ptree* currentTree = nullptr;

	const std::map<std::string, std::string> treeDefaultvalues = {
    {"connectionData26Numbers", "562847563758263a95836111484"},
		{"certificateFilePath", "./certs/domena.crt"},
		{"privateKeyFilePath", "./certs/domena.key"},
		{"maxNumberOfConnections", "100"},
		{"timeoutLimit", "10"}
	};

	struct WrongJsonDataException : std::runtime_error {
		WrongJsonDataException(const char* msg) : std::runtime_error(msg) {}
	};
};

#endif // ServerSettings_hpp
