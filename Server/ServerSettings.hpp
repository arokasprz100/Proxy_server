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

	void validateJsonFields() {
		if (currentTree != NULL) {
			std::string connectionData26Numbers = currentTree->get<std::string>("connectionData26Numbers");
			std::string maxNumberOfConnections = currentTree->get<std::string>("maxNumberOfConnections");
			std::string timeoutLimit = currentTree->get<std::string>("timeoutLimit");
			connectionData26Numbers.erase(std::remove_if(connectionData26Numbers.begin(),
																									 connectionData26Numbers.end(),
																									 ::isspace),
			 															connectionData26Numbers.end());
			try {

				if (connectionData26Numbers.length() < 26 || connectionData26Numbers.length() > 26)
					throw wrongJsonDataException();

				for (int i=0; i<connectionData26Numbers.length(); i++)
					if (isdigit(connectionData26Numbers[i]) == false)
						throw wrongJsonDataException();

				for (int i=0; i<maxNumberOfConnections.length(); i++)
					if (isdigit(maxNumberOfConnections[i]) == false)
						throw wrongJsonDataException();

				for (int i=0; i<timeoutLimit.length(); i++)
					if (isdigit(timeoutLimit[i]) == false)
						throw wrongJsonDataException();

			} catch (wrongJsonDataException& e) {
				// TODO: what should happen?
				std::cout << e.what() << std::endl;
			}

			setConnectionData26Numbers(connectionData26Numbers);
			setMaxNumberOfConnections(maxNumberOfConnections);
			setTimeoutLimit(timeoutLimit);
		}
	}

	void readJson(std::string jsonPath) {
		createCurrentTree();
		read_json(jsonPath, *currentTree);
		validateJsonFields();
	}

	// TODO: don't understand what's wrong with write_json
	// tried many different ways and nothing worked.
	void writeJson(std::string jsonPath) {
		if (currentTree == NULL) createCurrentTree();
		// write_json(jsonPath, currentTree);
	}


	// Debug function
	void printCurrentTree() {
		write_json(std::cout, *currentTree);
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

	// Setters for internal server use in order to modify
	// json values accordingly.
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

	// TODO: this is just sample data used for testing.
	// Needs to be filled in with actual default data as
	// this is used in the constructor.
	const std::vector<std::pair<std::string, std::string>> treeDefaultvalues = {
    {"connectionData26Numbers", "562847563758263a95836111484"},
		{"certificateFilePath", "./ca.crt"},
		{"privateKeyFilePath", "./key.crt"},
		{"maxNumberOfConnections", "3"},
		{"timeoutLimit", "4"},
	};

	struct wrongJsonDataException : public std::exception {
		const char* what() const throw() { return "Wrong data provided in Json input"; }
	};
};

#endif // ServerSettings_hpp
