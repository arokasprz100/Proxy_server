/**
*	@file HostDisconnector.hpp
*	@brief 
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
	*	
	*	@param client
	*	@see Client
	*	@param pollFDs
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