/**
*	@file ClientAcceptorChooser.hpp
*	@brief This file contains the definition of a function that picks connection acceptor based on the options the server is running.
*/

#ifndef ClientAcceptorChooser_hpp
#define ClientAcceptorChooser_hpp

#include "../Client.hpp"
#include "../ClientConnectionType.hpp"

#include <functional>
#include <map>
#include <utility>

/**
*	@typedef Acceptor An alias for clearer code.
*/
using Acceptor = std::function<std::tuple<Client, pollfd>(int, SSL_CTX*)>;

/**
*	@class ClientAcceptorChooser
*/
class ClientAcceptorChooser final
{
public:
	/**
	*	This member function implements picking appropriate connection acceptor for the client to proxy connection.
	*	@param clientConnectionType Value descirbing the type of client to proxy connection.
	*	@see ClientConnectionType 
	*	@returns Appropriate std::function object containing the acceptor for the given connection type.
	*/
	static Acceptor chooseClientAcceptor(ClientConnectionType clientConnectionType) {
		return CONNECTION_ACCEPTORS_BY_CONNECTION_TYPES[clientConnectionType];
	}

private:
	
	static std::map<ClientConnectionType, Acceptor> CONNECTION_ACCEPTORS_BY_CONNECTION_TYPES;

};


#endif // ClientAcceptorChooser_hpp