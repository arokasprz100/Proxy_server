/**
*	@file OperationStatusHandlerChooser.hpp
*	@brief This file contains the definition of a function that picks connection handler based on the client connection type.
*/

#ifndef OperationStatusHandlerChooser_hpp
#define OperationStatusHandlerChooser_hpp

#include <functional>
#include <map>

#include "../Client.hpp"
#include "../ClientConnectionType.hpp"

/**
*	@class OperationStatusHandlerChooser
*/
class OperationStatusHandlerChooser final 
{
public:
	/**
	*	This member function implements picking appropriate connection handler for the client to proxy connection.
	*	@param clientConnectionType Value descirbing the type of client to proxy connection.
	*	@see ClientConnectionType 
	*	@returns Appropriate std::function object containing the handler to the given connection type.
	*/
	static auto chooseOperationStatusHandler(ClientConnectionType clientConnectionType) {
		return OPERATION_STATUS_HANDLERS_BY_CLIENT_CONNECTION_TYPE[clientConnectionType];
	}

private:

	static std::map<ClientConnectionType, std::function<void(Client&, int, std::vector<pollfd>::iterator)>> 
		OPERATION_STATUS_HANDLERS_BY_CLIENT_CONNECTION_TYPE;

};

#endif // OperationStatusHandlerChooser_hpp