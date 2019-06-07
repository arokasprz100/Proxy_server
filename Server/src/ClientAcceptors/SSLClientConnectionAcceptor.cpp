#include "../../include/ClientAcceptors/SSLClientConnectionAcceptor.hpp"

std::tuple<Client, pollfd> SSLClientConnectionAcceptor::acceptConnection(int serverSocket, SSL_CTX* sslContext) 
{
	sockaddr_in addr;
	uint len = sizeof(addr);

	int clientSocket = accept(serverSocket, (sockaddr*)&addr, &len);
	exitOnError(clientSocket, "accept() function failed");
	
	SSL* newSsl = SSL_new(sslContext);
	SSL_set_fd(newSsl, clientSocket);
	SSL_set_accept_state(newSsl);
	
	setSocketToNonBlocking(clientSocket);

	Client client(newSsl, addr, clientSocket);

	return std::tuple<Client, pollfd>(client, {clientSocket, POLLIN | POLLOUT, 0});
}

void SSLClientConnectionAcceptor::setSocketToNonBlocking(int socketToSet)
{
	int flags = fcntl(socketToSet, F_GETFL);
	int operationStatus = fcntl(socketToSet, F_SETFL, O_NONBLOCK | flags);
	exitOnError(operationStatus, "fcntl() function failed");
}

void SSLClientConnectionAcceptor::exitOnError(int operationStatus, const std::string& errorMessage)
{
	if (operationStatus < 0) {
		perror(errorMessage.c_str());
		exit(EXIT_FAILURE);
	}
}