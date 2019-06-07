/**
*	@file UnencryptedClientConnectionAcceptor.hpp
*	@brief This file contains the definition of the plant HTTP connection to proxy acceptor.
*/

#ifndef UnencryptedClientConnectionAcceptor_hpp
#define UnencryptedClientConnectionAcceptor_hpp

#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <string>

/**
*	@class UnencryptedClientConnectionAcceptor
*/
class UnencryptedClientConnectionAcceptor final
{
public:
	/**
	*	This function accepts plain HTTP connection from client to the proxy and sets up the socket on which the proxy will be receiving data from client.
	*	@param serverSocket The server fd on which to accept a connection.
	*	@param sslContext A pointer to OpenSSL context. This isn't actually used but needs to be an argument to work with std::function the way we are using it.
	*	@returns A tuple containing new Client object and pollfd.
	*	@see Client
	*/
	static std::tuple<Client, pollfd> acceptConnection(int serverSocket, SSL_CTX* sslContext) {
		(void)sslContext; // disables warning

		sockaddr_in addr;
		uint len = sizeof(addr);

		int clientSocket = accept(serverSocket, (sockaddr*)&addr, &len);
		exitOnError(clientSocket, "accept() function failed");
		
		setSocketToNonBlocking(clientSocket);

		Client client(nullptr, addr, clientSocket);

		return std::tuple<Client, pollfd>(client, {clientSocket, POLLIN | POLLOUT, 0});
	}

private:

	static void setSocketToNonBlocking(int socketToSet)
	{
		int flags = fcntl(socketToSet, F_GETFL);
		int operationStatus = fcntl(socketToSet, F_SETFL, O_NONBLOCK | flags);
		exitOnError(operationStatus, "fcntl() function failed");
	}

	static void exitOnError(int operationStatus, const std::string& errorMessage)
	{
		if (operationStatus < 0) {
			perror(errorMessage.c_str());
			exit(EXIT_FAILURE);
		}
	}
};

#endif // UnencryptedClientConnectionAcceptor_hpp