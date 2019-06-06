#ifndef ClientDisconnector_hpp
#define ClientDisconnector_hpp

class ClientDisconnector
{
public:

	static void disconnect(Client& client, std::vector<pollfd>& pollFDs) {

		if (client.ssl) {
			SSL_free(client.ssl);
		}

		int clientFd = client.clientSocket;

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

};

#endif // ClientDisconnector_hpp