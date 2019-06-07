/**
*	@file ServerInitializer.hpp
*	@brief This file contians the definition of ServerInitializer class that sets up some key parts of the server.
*/

#ifndef ServerInitializer_hpp
#define ServerInitializer_hpp

#include <tuple>
#include <vector>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <poll.h>

/**
*	@class ServerInitializer
*/
class ServerInitializer final 
{
public:
	/**
	*	This member function sets up important variables and sockets needed for the server to work.
	*	@param port Port number on which the server will listen.
	*	@param pollFDs A reference std::vector of pollfd's in the Server class. This function puts the server's pollfd on the first index.
	*	@see Server
	*	@returns A std::tuple of the port on which the server is listening and server's IP address.
	*/
	static std::tuple<int, sockaddr_in> initialize(int port, std::vector<pollfd>& pollFDs);

private:

	static sockaddr_in prepareServerAddr(int port);

	static int prepareServerSocket();

	static void setSocketToNonBlocking(int socketToSet);

	static void setSocketOptions(int serverSocket);

	static void performBind(int serverSocket, sockaddr_in& serverAddr);

	static void performListen(int serverSocket);

	static void prepareServerPollFD(int serverSocket, std::vector<pollfd>& pollFDs);

	static void exitOnError(int operationStatus, const std::string& errorMessage);

};

#endif // ServerInitializer_hpp