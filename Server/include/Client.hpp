/**
*	@file Client.hpp
*	@brief This file contains the definition of the Client class.
*	
*	The Client class describes the connection of a browser client(or any other HTTP/SSL for that matter) to the destination
*	server. It accumulates data both from the destination server as well as the client.
*/

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

/**
*	@class Client class describes the connection between client and the destination server.
*/
class Client
{
public:
	/**
	*	A constructor, initializes the members of the class.
	*	@param ssl A pointer to SSL object from OpenSSL.
	*	@param sockaddr_in A struct initalized with accept() call.
	*	@param clientSocket A file descriptor to the socket the connection from client will happen.
	*/
	Client (SSL* ssl, sockaddr_in clientAddr, int clientSocket) : 
		clientAddr(clientAddr),
		ssl(ssl), 
		clientSocket(clientSocket),
		serverSocket(-1),
		id(nextClientID++),
		timestamp(std::chrono::high_resolution_clock::now()) {}

	/**
	*	Member function that clears data collected from both the client and destination server.
	*/
	void clearDataFromClient() {
		dataFromClient.clear();
		m_httpRequestFromClient.clear();
	}

	/**
	*	Member function that appends data collected from client to the data so far collecetd.
	*	@param dataToAdd Contains the data to be appended.
	*/
	void addDataFromClient(const std::vector<char>& dataToAdd) {
		dataFromClient.insert(dataFromClient.end(), dataToAdd.begin(), dataToAdd.end());
	}

	/**
	*	Member function that returns the data collected from client so far.
	*	@returns Data collected so far as reference.
	*/
	std::vector<char>& getDataFromClient() {
		return dataFromClient;
	}

	/**
	*	Member function that sets the connection type member to the destination server.
	*	@see connectionType	
	*	@param connectionType Is of enum type ConnectionType and describes the desired value to set.
	*	@see ConnectionType
	*/
	void setConnectionType(ConnectionType connectionType) {
		this->connectionType = connectionType;
	}

	/**
	*	Member function that returns the currently set connectionType.
	*	@see connectionType
	*	@returns A reference to the member value.
	*/
	const ConnectionType& getConnectionType() const {
		return connectionType;
	}

	/**
	*	Member function that returns the objects id.
	*	@see id
	*/
	int getID() const {
		return id;
	}

	/**
	*	Member function that sets current time to the timestamp member.
	*	@see timestamp
	*/
	void refreshTimestamp() {
		timestamp = std::chrono::high_resolution_clock::now();
	}

	/**
	*	Member function that returns the value of the timestamp member.
	*	@see timestamp
	*/
	const auto& getCurrentTimestamp() const {
		return timestamp;
	}

	/**
	*	Member function that returns client SSL.
	*	@see ssl
	*/
	SSL* getSSL() {
		return ssl;
	}

	/**
	*	Member function that returns client connection socket.
	*	@see clientSocket
	*/
	int getClientSocket() const {
		return clientSocket;
	}

	/**
	*	Member function that returns server connection socket.
	*	@see serverSocket
	*/
	int getServerSocket() const {
		return serverSocket;
	}

	/**
	*	Member function that sets server connection socket.
	*	@see serverSocket
	*/
	void setServerSocket(int serverSocket) {
		this->serverSocket = serverSocket;
	}

	/**
	*	@var m_httpRequestFromClient
	*	@brief 
	*/
	std::vector<char> m_httpRequestFromClient;

	/**
	*	@var m_httpResponseFromServer
	*	@brief 
	*/
	std::vector<char> m_httpResponseFromServer;

	/**
	*	@var serverAddr
	*	@brief Contains the address of the destination server(IPv4:port).
	*/
	sockaddr_in serverAddr;

	/**
	*	@var clientAddr
	*	@brief Contains the address of the client(IPv4:port).
	*/
	sockaddr_in clientAddr;

private:

	/**
	*	@var ssl
	*	@brief Contains the pointer to appropriate SSL object from OpenSSL needed to use OpenSSL functions when the connection to proxy is over SSL.
	*/
	SSL* ssl = nullptr;

	/**
	*	@var clientSocket
	*	@brief Contains the file descriptor to a socket which communicates with the client.
	*/
	int clientSocket;

	/**
	*	@var serverSocket
	*	@brief Contains the file descriptor to a socket which communicates with the destination server.
	*/
	int serverSocket;

	/**
	*	@var nextClientID
	*	@brief Static variable containing ID of next client (client counter).
	*/
	static int nextClientID;

	/**
	*	@var id
	*	@brief Client ID.
	*/
	int id;

	/**
	*	@var timestamp
	*	@brief Current timestamp - indicates laat data transaction.
	*/
	std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;

	/**
	*	@var connectionType
	*	@brief Contains client connection type.
	*/
	ConnectionType connectionType = ConnectionType::UNDEFINED;

	/**
	*	@var dataFromClient
	*	@brief Contains the data collected from client so far.
	*/
	std::vector<char> dataFromClient; 

};

#endif // Client_hpp