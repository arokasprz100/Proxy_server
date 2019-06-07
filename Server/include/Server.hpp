/**
*	@file Server.hpp
*	@brief This file contains the definition of the Server class that implements the server's functions.
*/

/**
*	@mainpage Proxy %Server
*	Proxy server created using C++.
*	@section compilation_section Compilation
*	To be able to use this software you need to compile it frist. To do so you first need to navigate to the Server folder.
*	Then you need to run: @code cmake CMakeLists.txt @endcode
*	This command will generate a proper Makefile along other files to compile the project.
*	Next just type: @code make @endcode
*	@warning To do this you need cmake version at least 3.9.6 as well as make installed on top of all the other libraries mentioned in the About section.
*	@section usage_section Usage
*	It's worth noting that the generated Makefile will have an additional target, namely, running:
*	@code make doc_doxygen @endcode
*	Will produce this documentation! Be aware that this doesnt check if the current one is up-to-date and will produce it from scratch.
*	Produced documentation is provided in HTML and LaTeX.
*	Compiled this program has multiple options of being run.
*	@li -h [--help]           To see the help page for this program
*	@li -e [--encrypted]      Use encrypted connection between client and proxy
*	@li -u [ --unencrypted]   Use unencrypted connection between client and proxy, this option is selected by default
*	@li -s [ --silent ]       Do not display log messages, this option is selected by defualt
*	@li -v [ --verbose ]      Display log messages to console
*	@li -m [ --menu ]         Display interactive settings menu. Disabled by default.
*	@section about_section About
*	Compilation of this program requires you to have some libraries installed
*	@li [Boost].multiprecision
*	@li [Boost].program_options
*	@li [Boost].property_tree
*	@li OpenSSL library
*	
*	On top of that a compiler supporting C++17's fold expressions and parameter pack is needed, such as g++ version 8.2.
*	If that is not your default compiler you can specify a flag to cmake to set it manually.
*	@code cmake -DCMAKE_CXX_COMPILER=g++-8.2 CMakeLists.txt @endcode
*	@authors <a href="https://github.com/dz1domin">Dominik Dziuba</a>, <a href="https://github.com/arokasprz100">Arkadiusz Kasprzak</a>, <a href="https://github.com/Robin-Bjornsdotter">Michalina Oleksy</a>
*/

#ifndef Server_hpp
#define Server_hpp

#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <poll.h>

#include <vector>
#include <string>
#include <functional>

#include "Initializers/SSLInitializer.hpp"
#include "Initializers/ServerInitializer.hpp"

#include "ServerSettings/ServerSettings.hpp"
#include "ClientConnectionType.hpp"

#include "HttpRequestParser/IPAndPortExtractor.hpp"
#include "HostConnector.hpp"

#include "Client.hpp"
#include "ConnectionType.hpp"

/**
*	@class Server
*/
class Server
{
public:

	/**
	*	Server constructor. Creates core object that contains main program loop.
	*	@param port Server port.
	*	@param clientConnectionType Variable that indicates if server should use OpenSSL.
	*	@param serverSettings Object that contains server settings like max connection timeout.
	*/
	Server(int port, ClientConnectionType clientConnectionType, const ServerSettings& serverSettings);

	/**
	*	Destructor. Disconnects all clients.
	*/
	~Server();

	/**
	*	Main program loop. Accepts new connections, handles requests and responses. 
	*/
	void startHandlingClients();

private: // methods

	std::vector<pollfd>::iterator getPollFD(int socketNumber);

	void exitOnError(int operationStatus, const std::string& errorMessage);

	void performPoll();

	void acceptNewConnection();

	void closeMarkedConnections();

	void closeConnection(Client& client, int clientIndex);

private: // state

	int m_serverSocket;
	sockaddr_in m_serverAddr;
	SSL_CTX* m_ctx;

	const ServerSettings& serverSettings;

	std::vector<pollfd> m_pollfds;
	std::vector<Client> m_clients;
	std::vector<int> connectionsToClose;


	std::function<int(Client&)> unencryptedDataFromClientReader;
	std::function<int(Client&)> encryptedDataFromClientReader;
	std::function<int(Client&)> dataToClientWriter;
	std::function<void(Client&, int, std::vector<pollfd>::iterator)> operationStatusHandler;
	std::function<std::tuple<Client, pollfd>(int, SSL_CTX*)> clientAcceptor;
};


#endif // Server_hpp