#ifndef ServerSettings_hpp
#define ServerSettings_hpp

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <exception>

using namespace boost::property_tree;
class ServerSettings
{


public:

	ServerSettings() { setCurrentTreeToDefault(); }

	void deleteCurrentTree() {
		if (currentTree != NULL) {
			delete currentTree;
			currentTree = NULL;
		}
	}

	void createCurrentTree() {
		if (currentTree != NULL) deleteCurrentTree();
		currentTree = new ptree();
	}

	void setCurrentTreeToDefault() {
		createCurrentTree();
		for (auto const& i: treeDefaultvalues) {
			currentTree->put(i.first, i.second);
		}
	}

	// Debug function
	void printCurrentTree() {
		write_json(std::cout, *currentTree);
	}

	void readJson(std::string jsonPath) {
		createCurrentTree();
		read_json(jsonPath, *currentTree);
		processNumbers();
	}

	void processNumbers() {
		if (currentTree != NULL) {
			std::string connectionData26Numbers = currentTree->get<std::string>("connectionData26Numbers");
			std::string maxNumberOfConnections = currentTree->get<std::string>("maxNumberOfConnections");
			std::string timeoutLimit = currentTree->get<std::string>("timeoutLimit");
			connectionData26Numbers.erase(std::remove_if(connectionData26Numbers.begin(),
																									 connectionData26Numbers.end(),
																									 ::isspace),
			 															connectionData26Numbers.end());
			// poprawie to bo jest chujowe
			for (int i=0; i<connectionData26Numbers.length(); i++) {
				if (isdigit(connectionData26Numbers[i]) == false) {
					std::cout << "wrong connectionData26Numbers" << std::endl; //debug
				} // Exception
			}
			if (connectionData26Numbers.length() < 26 || connectionData26Numbers.length() > 26) {
				std::cout << "wrong connectionData26Numbers" << std::endl; //debug
				// Exception
			}
			for (int i=0; i<maxNumberOfConnections.length(); i++) {
				if (isdigit(maxNumberOfConnections[i]) == false) {
					std::cout << "wrong maxNumberOfConnections" << std::endl; //debug
				} // Exception
			}
			for (int i=0; i<timeoutLimit.length(); i++) {
				if (isdigit(timeoutLimit[i]) == false) {
					std::cout << "wrong timeoutLimit" << std::endl; //debug
				} // Exception
			}
			setConnectionData26Numbers(connectionData26Numbers);
			setMaxNumberOfConnections(maxNumberOfConnections);
			setTimeoutLimit(timeoutLimit);
		}
	}

	std::string getConnectionData26Numbers() {
		if (currentTree != NULL) return currentTree->get<std::string>("connectionData26Numbers");
		else return NULL;
	}
	std::string getCertificateFilePath() {
		if (currentTree != NULL) return currentTree->get<std::string>("certificateFilePath");
		else return NULL;
	}
	std::string getPrivateKeyFilePath() {
		if (currentTree != NULL) return currentTree->get<std::string>("privateKeyFilePath");
		else return NULL;
	}
	std::string getMaxNumberOfConnections() {
		if (currentTree != NULL) return currentTree->get<std::string>("maxNumberOfConnections");
		else return NULL;
	}
	std::string getTimeoutLimit() {
		if (currentTree != NULL) return currentTree->get<std::string>("timeoutLimit");
		else return NULL;
	}

	void setConnectionData26Numbers(std::string connectionData26Numbers) {
		if (currentTree != NULL) currentTree->put("connectionData26Numbers", connectionData26Numbers);
	}
	void setCertificateFilePath(std::string certificateFilePath) {
		if (currentTree != NULL) currentTree->put("certificateFilePath", certificateFilePath);
	}
	void setPrivateKeyFilePath(std::string privateKeyFilePath) {
		if (currentTree != NULL) currentTree->put("privateKeyFilePath", privateKeyFilePath);
	}
	void setMaxNumberOfConnections(std::string maxNumberOfConnections) {
		if (currentTree != NULL) currentTree->put("maxNumberOfConnections", maxNumberOfConnections);
	}
	void setTimeoutLimit(std::string timeoutLimit) {
		if (currentTree != NULL) currentTree->put("timeoutLimit", timeoutLimit);
	}


private:
	ptree* currentTree = NULL;

	const std::vector<std::pair<std::string, std::string>> treeDefaultvalues = {
    {"connectionData26Numbers", "56284756375826395836111484"},
		{"certificateFilePath", "./ca.crt"},
		{"privateKeyFilePath", "./key.crt"},
		{"maxNumberOfConnections", "3"},
		{"timeoutLimit", "4"},
	};
};

#endif // ServerSettings_hpp
