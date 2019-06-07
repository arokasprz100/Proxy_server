#include "../../include/Disconnectors/HostDisconnector.hpp"

void HostDisconnector::disconnect(Client& client, std::vector<pollfd>& pollFDs) 
{
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