#include "../../include/Disconnectors/ClientDisconnector.hpp"

void ClientDisconnector::disconnect(Client& client, std::vector<pollfd>& pollFDs) 
{
	if (client.getSSL()) {
		SSL_free(client.getSSL());
	}

	int clientFd = client.getClientSocket();

	for(long unsigned int pollFdIndex = 0; pollFdIndex < pollFDs.size(); ++pollFdIndex)
	{
		auto& pollFd = pollFDs[pollFdIndex];
		if(pollFd.fd == clientFd)
		{
			close(pollFd.fd);
			pollFDs.erase(pollFDs.begin() + pollFdIndex);
			break;
		}
	}
}