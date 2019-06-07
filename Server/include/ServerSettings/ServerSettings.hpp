/**
*	@file ServerSettings.hpp
*	@brief This file contains class that contains server settings (like timeout limit).
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
	ServerSettings();

	/**
	*	Basic destructor.
	*/
	~ServerSettings();

	/**
	*	This member function frees memory allocated for the property tree.
	*/
	void deleteCurrentTree();

	/**
	*	This member function creates new property tree.
	*/
	void createCurrentTree();

	/**
	*	This member function sets the property tree to default values.
	*/
	void setCurrentTreeToDefault();

	/**
	*	This member function checks whether information read from the json file is correct.
	*	@returns True if file is correct and false if not.
	*/
	bool validateJsonFields();

	/**
	*	This member function reads given json settings file and validates it.
	*	@param jsonPath	A string path to the settings file.
	*/
	void readJson(const std::string& jsonPath);

	/**
	*	This member function changes the contents of the json settings file if any were made during interactive command line session.
	*	@param jsonPath A string path to the settings file.
	*/
	void writeJson(const std::string& jsonPath);

	/**
	*	Debug function. Prints current property tree.
	*/
	void printCurrentTree();

	/**
	*	A getter function.
	*	@returns A string object containing the sequence to be in the sequence swapping function. Read from the json settings file.
	*	@see PSiCAlgorithm
	*/
	std::string getConnectionData26Numbers() const;

	/**
	*	A getter function.
	*	@returns A string object containing the path to the proxy server's certificate file. Read from the json settings file.
	*/
	std::string getCertificateFilePath() const;

	/**
	*	A getter function.
	*	@returns A string object containing the path to the proxy server's private key file. Read from the json settings file.
	*/
	std::string getPrivateKeyFilePath() const;

	/**
	*	A getter function.
	*	@returns The maximum number of connections the server can handle at a time. Read from the json settings file.
	*/
	unsigned getMaxNumberOfConnections() const;

	/**
	*	A getter function.
	*	@returns The maximum number of seconds a connection through the proxy can be idle. Read from the json settings file.
	*/
	unsigned getTimeoutLimit() const;

	// Setters for internal server use in order to modify
	// json values accordingly.
	/**
	*	A setter function.
	*	@param connectionData26Numbers A reference to a string object containing the new sequence to be used.
	*/
	void setConnectionData26Numbers(const std::string& connectionData26Numbers);

	/**
	*	A setter function.
	*	@param certificateFilePath A reference to a string object containing the path to the proxy server's certificate file.
	*/
	void setCertificateFilePath(const std::string& certificateFilePath);

	/**
	*	A setter function.
	*	@param privateKeyFilePath A reference to a string object containing the path to the proxy server's private key file.
	*/
	void setPrivateKeyFilePath(const std::string& privateKeyFilePath);

	/**
	*	A setter function.
	*	@param maxNumberOfConnections A reference to a string object containing the desired maximum connections to be used.
	*/
	void setMaxNumberOfConnections(const std::string& maxNumberOfConnections);

	/**
	*	A setter function.
	*	@param timeoutLimit A reference to a string object containing the desired maximum idle time of a connection.
	*/
	void setTimeoutLimit(const std::string& timeoutLimit);

private:
	ptree* currentTree = nullptr;

	const std::map<std::string, std::string> treeDefaultvalues = {
    {"connectionData26Numbers", "562847563758263a95836111484"},
		{"certificateFilePath", "./certs/domena.crt"},
		{"privateKeyFilePath", "./certs/domena.key"},
		{"maxNumberOfConnections", "100"},
		{"timeoutLimit", "60"}
	};

	struct WrongJsonDataException : std::runtime_error {
		WrongJsonDataException(const char* msg) : std::runtime_error(msg) {}
	};
};

#endif // ServerSettings_hpp
