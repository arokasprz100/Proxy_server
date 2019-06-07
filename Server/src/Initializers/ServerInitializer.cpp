#include "../../include/Initializers/ServerInitializer.hpp"

std::tuple<int, sockaddr_in> ServerInitializer::initialize(int port, std::vector<pollfd>& pollFDs) 
{
	sockaddr_in serverAddr = prepareServerAddr(port);
	int serverSocket = prepareServerSocket();
	setSocketToNonBlocking(serverSocket);
	setSocketOptions(serverSocket);
	performBind(serverSocket, serverAddr);
	performListen(serverSocket);
	prepareServerPollFD(serverSocket, pollFDs);

	return std::make_tuple(serverSocket, serverAddr);
}

sockaddr_in ServerInitializer::prepareServerAddr(int port) 
{	
	sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	return serverAddr;
}

int ServerInitializer::prepareServerSocket() 
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	exitOnError(serverSocket, "Unable to create socket");
	return serverSocket;
}

void ServerInitializer::setSocketToNonBlocking(int socketToSet)
{
	int flags = fcntl(socketToSet, F_GETFL);
	int operationStatus = fcntl(socketToSet, F_SETFL, O_NONBLOCK | flags);
	exitOnError(operationStatus, "fcntl() function failed");
}

void ServerInitializer::setSocketOptions(int serverSocket) 
{
	constexpr int one = 1;
	int operationStatus = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
	exitOnError(operationStatus, "Unable to set socket options");
}

void ServerInitializer::performBind(int serverSocket, sockaddr_in& serverAddr)
{
	int operationStatus = bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	exitOnError(operationStatus, "Unable to bind");
}

void ServerInitializer::performListen(int serverSocket) 
{
	int operationStatus = listen(serverSocket, 1);
	exitOnError(operationStatus, "Unable to listen");
}

void ServerInitializer::prepareServerPollFD(int serverSocket, std::vector<pollfd>& pollFDs)
{
	pollFDs.push_back({serverSocket, POLLIN, 0});
}

void ServerInitializer::exitOnError(int operationStatus, const std::string& errorMessage)
{
	if (operationStatus < 0) {
		perror(errorMessage.c_str());
		exit(EXIT_FAILURE);
	}
}