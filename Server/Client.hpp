#ifndef Client_hpp
#define Client_hpp

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

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

	Client (SSL* ssl, sockaddr_in clientAddr, int clientSocket) : 
		ssl(ssl), 
		clientSocket(clientSocket),
		serverSocket(-1),
		m_clientAddr(clientAddr),
		id(nextClientID++),
		timestamp(std::chrono::high_resolution_clock::now()) {}

	void clearDataFromClient() {
		m_dataFromClient.clear();
		m_httpRequestFromClient.clear();
	}

	void addDataFromClient(const std::vector<char>& dataToAdd) {
		m_dataFromClient.insert(m_dataFromClient.end(), dataToAdd.begin(), dataToAdd.end());
	}

	std::vector<char>& getDataFromClient() {
		return m_dataFromClient;
	}

	void setConnectionType(ConnectionType connectionType) {
		this->connectionType = connectionType;
	}

	const ConnectionType& getConnectionType() const {
		return connectionType;
	}

	int getID() const {
		return id;
	}

	void refreshTimestamp() {
		timestamp = std::chrono::high_resolution_clock::now();
	}

	const auto& getCurrentTimestamp() const {
		return timestamp;
	}

	SSL* ssl = nullptr;

	int clientSocket;
	int serverSocket;

	sockaddr_in m_serverAddr;
	sockaddr_in m_clientAddr;

	std::vector<char> m_dataFromClient; 

	std::vector<char> m_httpRequestFromClient;
	std::vector<char> m_httpResponseFromServer;

private:

	static int nextClientID;

	int id;
	std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;
	ConnectionType connectionType = ConnectionType::UNDEFINED;

};

#endif // Client_hpp