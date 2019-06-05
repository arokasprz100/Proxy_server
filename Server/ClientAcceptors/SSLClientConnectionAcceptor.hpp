#ifndef SSLClientConnectionAcceptor_hpp
#define SSLClientConnectionAcceptor_hpp

#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <string>

class SSLClientConnectionAcceptor final
{
public:

	static std::tuple<Client, pollfd> acceptConnection(int serverSocket, SSL_CTX* sslContext) {

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

#endif // SSLClientConnectionAcceptor_hpp