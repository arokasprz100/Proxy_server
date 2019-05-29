#ifndef HostConnector_hpp
#define HostConnector_hpp

#include "Client.hpp"

#include <string>
#include <iostream>

class Server;

class HostConnector
{
public:

	// TODO: remove Server from arguments
	static void connectToHost(Client& client, const std::string& hostIp, int port, Server& server);
};

#endif // HostConnector_hpp