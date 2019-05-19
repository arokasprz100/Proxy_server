#ifndef SSLDataReader_hpp
#define SSLDataReader_hpp

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <iostream>

#include "Client.hpp"
#include "ServerSettings.hpp"

class SSLDataReader final
{
public:

	static int read(Client& client) 
	{
		char buffer[ServerSettings::BUFFER_SIZE] = {'\0'};
		int operationStatus = SSL_read(client.ssl, buffer, ServerSettings::BUFFER_SIZE);
		if (operationStatus == 0) {
			std::cout << "No data to read" << std::endl;
		}

		client.addDataFromClient(buffer);

		while (SSL_pending(client.ssl)) 
		{
			for (auto& characterInBuffer : buffer) {
				characterInBuffer = '\0';
			}
			operationStatus = SSL_read(client.ssl, buffer, ServerSettings::BUFFER_SIZE);
			client.addDataFromClient(buffer);
		}
		return operationStatus;

	}


};


#endif // SSLDataReader_hpp