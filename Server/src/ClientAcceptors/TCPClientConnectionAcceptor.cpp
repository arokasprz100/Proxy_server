#include "../../include/ClientAcceptors/TCPClientConnectionAcceptor.hpp"

std::tuple<Client, pollfd> TCPClientConnectionAcceptor::acceptConnection(int serverSocket, SSL_CTX* sslContext) 
{
	(void)sslContext; // disables warning

	sockaddr_in addr;
	uint len = sizeof(addr);

	int clientSocket = accept(serverSocket, (sockaddr*)&addr, &len);
	exitOnError(clientSocket, "accept() function failed");
	
	setSocketToNonBlocking(clientSocket);

	Client client(nullptr, addr, clientSocket);

	return std::tuple<Client, pollfd>(client, {clientSocket, POLLIN | POLLOUT, 0});
}

void TCPClientConnectionAcceptor::setSocketToNonBlocking(int socketToSet)
{
	int flags = fcntl(socketToSet, F_GETFL);
	int operationStatus = fcntl(socketToSet, F_SETFL, O_NONBLOCK | flags);
	exitOnError(operationStatus, "fcntl() function failed");
}

void TCPClientConnectionAcceptor::exitOnError(int operationStatus, const std::string& errorMessage)
{
	if (operationStatus < 0) {
		perror(errorMessage.c_str());
		exit(EXIT_FAILURE);
	}
}