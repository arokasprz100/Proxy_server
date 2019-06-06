#include "../include/HostConnector.hpp"
#include "../include/Server.hpp"
#include "../include/LogSystem/LogSystem.hpp"

pollfd HostConnector::connectToHost(Client& client, const std::string& hostIp, int port) {

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

	LogSystem::logMessage("Connected successfully", "NEW CONNECTION", std::to_string(client.getID()));

	return {client.serverSocket, POLLIN | POLLOUT, 0};
}