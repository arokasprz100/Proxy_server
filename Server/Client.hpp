#ifndef Client_hpp
#define Client_hpp

#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <poll.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "ConnectionType.hpp"

class Client
{
public:

	Client (SSL* ssl, sockaddr_in clientAddr) 
		: ssl(ssl), m_clientAddr(clientAddr)
	{
		std::cout << "New client created" << std::endl;
	}

	void clearDataFromClient() {
		m_dataFromClient.clear();
		m_httpRequestFromClient.clear();
	}

	void addDataFromClient(const std::vector<char>& dataToAdd) {
		m_dataFromClient.insert(m_dataFromClient.end(), dataToAdd.begin(), dataToAdd.end());
	}

	const std::vector<char> getDataFromClient() const {
		return m_dataFromClient;
	}

	SSL* ssl;

	int clientSocket;
	int serverSocket;

	sockaddr_in m_serverAddr;
	sockaddr_in m_clientAddr;

	ConnectionType connectionType = ConnectionType::UNDEFINED;

	pollfd* clientConnectionPollFD = nullptr;
	pollfd* serverConnectionPollFD = nullptr;

	std::vector<char> m_dataFromClient; 

	std::vector<char> m_httpRequestFromClient;
	std::vector<char> m_httpResponseFromServer;

	int id;
};

#endif // Client_hpp