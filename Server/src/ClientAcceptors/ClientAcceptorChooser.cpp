#include "../../include/ClientAcceptors/ClientAcceptorChooser.hpp"

#include "../../include/ClientAcceptors/SSLClientConnectionAcceptor.hpp"
#include "../../include/ClientAcceptors/UnencryptedClientConnectionAcceptor.hpp"

std::map<ClientConnectionType, Acceptor> 
	ClientAcceptorChooser::CONNECTION_ACCEPTORS_BY_CONNECTION_TYPES = {
		{ ClientConnectionType::ENCRYPTED, [](int serverSocket, SSL_CTX* sslContext) { 
			return SSLClientConnectionAcceptor::acceptConnection(serverSocket, sslContext); }},
		{ ClientConnectionType::UNENCRYPTED, [](int serverSocket, SSL_CTX* sslContext) {
			return UnencryptedClientConnectionAcceptor::acceptConnection(serverSocket, sslContext); }}
};