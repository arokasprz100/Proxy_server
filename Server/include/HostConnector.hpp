/**
*	@file HostConnector.hpp
*	@brief This file contains the definition of a class that implements connecting to destination server.
*/

#ifndef HostConnector_hpp
#define HostConnector_hpp

#include "Client.hpp"

#include <string>
#include <iostream>

class Server;

/**
*	@class HostConnector
*/
class HostConnector
{
public:
	/**
	*	This function connects the proxy to the destination server.
	*	@param client A reference to a Client object. Sets values to necessary members.
	*	@see Client
	*	@param hostIp A reference to a std::string object containing the destination server's IP in dot notation.
	*	@param port An int describing the destination server's port to try to connect to.
	*/
	static pollfd connectToHost(Client& client, const std::string& hostIp, int port);
};

#endif // HostConnector_hpp