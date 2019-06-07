/**
*	@file ServerSettings.hpp
*	@brief
*/

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

/**
*	@class ServerSettings
*/
class ServerSettings final 
{
public:
	/**
	*	Basic constructor.
	*/
	ServerSettings() { 
		setCurrentTreeToDefault(); 
	}

	/**
	*	Basic destructor.
	*/
	~ServerSettings() {
		deleteCurrentTree();
	}

	/**
	*	This member function frees memory allocated for the property tree.
	*/
	void deleteCurrentTree() {
		if (currentTree != nullptr) {
			delete currentTree;
			currentTree = nullptr;
		}
	}

	/**
	*	This member function creates new property tree.
	*/
	void createCurrentTree() {
		if (currentTree != nullptr) deleteCurrentTree();
		currentTree = new ptree();
	}

	/**
	*	This member function sets the property tree to default values.
	*/
	void setCurrentTreeToDefault() {
		createCurrentTree();
		for (auto const& i: treeDefaultvalues) {
			currentTree->put(i.first, i.second);
		}
	}

	/**
	*	This member function checks whether information read from the json file is correct.
	*	@returns True if file is correct and false if not.
	*/
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

	/**
	*	This member function reads given json settings file and validates it.
	*	@param jsonPath	A string path to the settings file.
	*/
	void readJson(std::string jsonPath) {
		createCurrentTree();
		read_json(jsonPath, *currentTree);
		validateJsonFields();
	}

	/**
	*	This member function changes the contents of the json settings file if any were made during interactive command line session.
	*	@param jsonPath A string path to the settings file.
	*/
	void writeJson(const std::string& jsonPath) {
		if (currentTree == nullptr) createCurrentTree();
		write_json(jsonPath, *currentTree);
	}


	/**
	*	Debug function. Prints current property tree.
	*/
	void printCurrentTree() {
		write_json(std::cout, *currentTree);
	}

	/**
	*	A getter function.
	*	@returns A string object containing the sequence to be in the sequence swapping function. Read from the json settings file.
	*	@see PSiCAlgorithm
	*/
	std::string getConnectionData26Numbers() const {
		if (currentTree != nullptr) {
			return currentTree->get<std::string>("connectionData26Numbers");
		}
		else return nullptr;
	}

	/**
	*	A getter function.
	*	@returns A string object containing the path to the proxy server's certificate file. Read from the json settings file.
	*/
	std::string getCertificateFilePath() const {
		if (currentTree != nullptr) {
			return currentTree->get<std::string>("certificateFilePath");
		}
		else return nullptr;
	}

	/**
	*	A getter function.
	*	@returns A string object containing the path to the proxy server's private key file. Read from the json settings file.
	*/
	std::string getPrivateKeyFilePath() const {
		if (currentTree != nullptr) {
			return currentTree->get<std::string>("privateKeyFilePath");
		}
		else return nullptr;
	}

	/**
	*	A getter function.
	*	@returns The maximum number of connections the server can handle at a time. Read from the json settings file.
	*/
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

	/**
	*	A getter function.
	*	@returns The maximum number of seconds a connection through the proxy can be idle. Read from the json settings file.
	*/
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
	/**
	*	A setter function.
	*	@param connectionData26Numbers A reference to a string object containing the new sequence to be used.
	*/
	void setConnectionData26Numbers(const std::string& connectionData26Numbers) {
		if (currentTree != nullptr) {
			currentTree->put("connectionData26Numbers", connectionData26Numbers);
		}
	}

	/**
	*	A setter function.
	*	@param certificateFilePath A reference to a string object containing the path to the proxy server's certificate file.
	*/
	void setCertificateFilePath(const std::string& certificateFilePath) {
		if (currentTree != nullptr) {
			currentTree->put("certificateFilePath", certificateFilePath);
		}
	}

	/**
	*	A setter function.
	*	@param privateKeyFilePath A reference to a string object containing the path to the proxy server's private key file.
	*/
	void setPrivateKeyFilePath(const std::string& privateKeyFilePath) {
		if (currentTree != nullptr) {
			currentTree->put("privateKeyFilePath", privateKeyFilePath);
		}
	}

	/**
	*	A setter function.
	*	@param maxNumberOfConnections A reference to a string object containing the desired maximum connections to be used.
	*/
	void setMaxNumberOfConnections(const std::string& maxNumberOfConnections) {
		if (currentTree != nullptr) {
			currentTree->put("maxNumberOfConnections", maxNumberOfConnections);
		}
	}

	/**
	*	A setter function.
	*	@param timeoutLimit A reference to a string object containing the desired maximum idle time of a connection.
	*/
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
