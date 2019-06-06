#ifndef HostConnector_hpp
#define HostConnector_hpp

#include "Client.hpp"

#include <string>
#include <iostream>

class Server;

class HostConnector
{
public:

	static pollfd connectToHost(Client& client, const std::string& hostIp, int port);
};

#endif // HostConnector_hpp