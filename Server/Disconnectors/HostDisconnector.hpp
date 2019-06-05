#ifndef HostDisconnector_hpp
#define HostDisconnector_hpp

class HostDisconnector final
{
public:

	static void disconnect(Client& client, std::vector<pollfd>& pollFDs) {
		int serverFd = client.serverSocket;

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