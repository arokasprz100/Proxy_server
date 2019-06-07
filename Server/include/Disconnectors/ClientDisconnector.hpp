/**
*	@file ClientDisconnector.hpp
*	@brief This file contains definition of a class that disconnects the client from the proxy and removes appropriate pollfd from server's pollfd std::vector.
*/


#ifndef ClientDisconnector_hpp
#define ClientDisconnector_hpp

/**
*	@class ClientDisconnector
*/
class ClientDisconnector final
{
public:
	/**
	*	This member function disconnects a given client from proxy and removes its pollfd from server's std::vector.
	*	@param client A reference to a Client object from server's std::vector of Client's.
	*	@see Client
	*	@param pollFDs A reference to server's std::vector of pollfd's.
	*/
	static void disconnect(Client& client, std::vector<pollfd>& pollFDs) {

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

};

#endif // ClientDisconnector_hpp