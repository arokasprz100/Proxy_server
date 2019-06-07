/**
*	@file TCPClientConnectionAcceptor.hpp
*	@brief This file contains the definition of the plant HTTP connection to proxy acceptor.
*/

#ifndef TCPClientConnectionAcceptor_hpp
#define TCPClientConnectionAcceptor_hpp

#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "../Client.hpp"

#include <string>
#include <tuple>

/**
*	@class TCPClientConnectionAcceptor
*/
class TCPClientConnectionAcceptor final
{
public:
	/**
	*	This function accepts plain HTTP connection from client to the proxy and sets up the socket on which the proxy will be receiving data from client.
	*	@param serverSocket The server fd on which to accept a connection.
	*	@param sslContext A pointer to OpenSSL context. This isn't actually used but needs to be an argument to work with std::function the way we are using it.
	*	@returns A tuple containing new Client object and pollfd.
	*	@see Client
	*/
	static std::tuple<Client, pollfd> acceptConnection(int serverSocket, SSL_CTX* sslContext);

private:

	static void setSocketToNonBlocking(int socketToSet);
	
	static void exitOnError(int operationStatus, const std::string& errorMessage);
};

#endif // TCPClientConnectionAcceptor_hpp