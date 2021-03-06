/**
*	@file SSLClientConnectionAcceptor.hpp
*	@brief This file contains the definition of the SSL connection to proxy acceptor.
*/

#ifndef SSLClientConnectionAcceptor_hpp
#define SSLClientConnectionAcceptor_hpp

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
*	@class SSLClientConnectionAcceptor
*/
class SSLClientConnectionAcceptor final
{
public:
	/**
	*	This function accepts SSL connection from client to the proxy and sets up the socket on which the proxy will be receiving data from client.
	*	@param serverSocket The server fd on which to accept a connection.
	*	@param sslContext A pointer to OpenSSL context.
	*	@returns A tuple containing new Client object and pollfd.
	*	@see Client
	*/
	static std::tuple<Client, pollfd> acceptConnection(int serverSocket, SSL_CTX* sslContext);

private:

	static void setSocketToNonBlocking(int socketToSet);

	static void exitOnError(int operationStatus, const std::string& errorMessage);

};

#endif // SSLClientConnectionAcceptor_hpp