#ifndef ClientAcceptorChooser_hpp
#define ClientAcceptorChooser_hpp

#include "../Client.hpp"
#include "../ClientConnectionType.hpp"

#include <functional>
#include <map>
#include <utility>

using Acceptor = std::function<std::tuple<Client, pollfd>(int, SSL_CTX*)>;

class ClientAcceptorChooser final
{
public:

	static Acceptor chooseClientAcceptor(ClientConnectionType clientConnectionType) {
		return CONNECTION_ACCEPTORS_BY_CONNECTION_TYPES[clientConnectionType];
	}

private:
	
	static std::map<ClientConnectionType, Acceptor> CONNECTION_ACCEPTORS_BY_CONNECTION_TYPES;

};


#endif // ClientAcceptorChooser_hpp