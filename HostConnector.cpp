#include "HostConnector.hpp"
#include "Server.hpp"

void HostConnector::connectToHost(Client& client, const std::string& hostIp, int port, Server& server) {

	client.serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (client.serverSocket== -1) {
		perror("socket() function failed");
		exit(1);
	}

	memset(&(client.m_serverAddr), 0, sizeof(client.m_serverAddr));
	client.m_serverAddr.sin_family = AF_INET;
	client.m_serverAddr.sin_port = htons(port);
	client.m_serverAddr.sin_addr.s_addr = inet_addr(hostIp.c_str());

	int flags = fcntl(client.serverSocket, F_GETFL, 0);
	int fcntl_status = fcntl(client.serverSocket, F_SETFL, flags | O_NONBLOCK);
	if (fcntl_status == -1) {
		perror("fcntl() function failed");
		exit(1);
	}

	int connect_status = connect(client.serverSocket, 
		reinterpret_cast<sockaddr*>(&(client.m_serverAddr)), 
		sizeof(client.m_serverAddr));
	if (connect_status >= 0) {
		perror("connect() function failed");
		exit(1);
	}

	server.m_pollfds.push_back({client.serverSocket, POLLIN | POLLOUT, 0});
	client.serverConnectionPollFD = &(server.m_pollfds.back());

	std::cout << "Connected successfully" << std::endl;
}