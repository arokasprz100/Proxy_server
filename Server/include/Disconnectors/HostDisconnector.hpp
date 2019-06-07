/**
*	@file HostDisconnector.hpp
*	@brief This file contains definition of a class that disconnects the proxy from the destination server and removes appropriate pollfd from server's pollfd std::vector.
*/

#ifndef HostDisconnector_hpp
#define HostDisconnector_hpp

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
	static void disconnect(Client& client, std::vector<pollfd>& pollFDs) {
		int serverFd = client.getServerSocket();

		for(long unsigned int pollFdIndex = 0; pollFdIndex < pollFDs.size(); ++pollFdIndex)
		{
			auto& pollFd = pollFDs[pollFdIndex];
			if(pollFd.fd == serverFd)
			{
				close(pollFd.fd);
				pollFDs.erase(pollFDs.begin() + pollFdIndex);
				break;
			}
		}
	}
};

#endif // HostDisconnector_hpp