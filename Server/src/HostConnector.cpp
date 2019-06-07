#include "../include/HostConnector.hpp"
#include "../include/LogSystem/LogSystem.hpp"

pollfd HostConnector::connectToHost(Client& client, const std::string& hostIp, int port) {

	client.setServerSocket(socket(AF_INET, SOCK_STREAM, 0));
	if (client.getServerSocket() == -1) {
		throw std::runtime_error("502");
	}

	memset(&(client.serverAddr), 0, sizeof(client.serverAddr));
	client.serverAddr.sin_family = AF_INET;
	client.serverAddr.sin_port = htons(port);
	client.serverAddr.sin_addr.s_addr = inet_addr(hostIp.c_str());

	int flags = fcntl(client.getServerSocket(), F_GETFL, 0);
	int fcntl_status = fcntl(client.getServerSocket(), F_SETFL, flags | O_NONBLOCK);
	if (fcntl_status == -1) {
		throw std::runtime_error("502");
	}

	int connect_status = connect(client.getServerSocket(), 
		reinterpret_cast<sockaddr*>(&(client.serverAddr)), 
		sizeof(client.serverAddr));
	if (connect_status >= 0) {
		throw std::runtime_error("502");
	}

	LogSystem::logMessage("Connected successfully", "NEW CONNECTION", std::to_string(client.getID()));

	return {client.getServerSocket(), POLLIN | POLLOUT, 0};
}