/**
*	@file HostDisconnector.hpp
*	@brief This file contains definition of a class that disconnects the proxy from the destination server and removes appropriate pollfd from server's pollfd std::vector.
*/

#ifndef HostDisconnector_hpp
#define HostDisconnector_hpp

#include <poll.h>
#include <vector>
#include "../Client.hpp"

/**
*	@class HostDisconnector
*/
class HostDisconnector final
{
public:
	/**
	*	This member function disconnects a given client from destination server and removes its pollfd from server's std::vector.
	*	@param client A reference to a Client object from server's std::vector of Client's.
	*	@see Client
	*	@param pollFDs A reference to server's std::vector of pollfd's.
	*/
	static void disconnect(Client& client, std::vector<pollfd>& pollFDs);
	
};

#endif // HostDisconnector_hpp