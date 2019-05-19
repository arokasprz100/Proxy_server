#ifndef Server_hpp
#define Server_hpp

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <poll.h>

#include <vector>
#include <string>
#include <iostream>

#include "SSLInitializer.hpp"
#include "SSLDataReader.hpp"
#include "SSLDataWriter.hpp"
#include "Client.hpp"


class Server
{
public:

	Server(int port)
	{	
		std::cout << " ==== Starting new server ==== " << std::endl;

		prepareServerAddr(port);
		prepareServerSocket();
		setSocketToNonBlocking(m_serverSocket);
		setSocketOptions();
		performBind();
		performListen();
		prepareServerPollFD();

		m_ctx = SSLInitializer::initializeOpenSSL();

		std::cout << " ==== Server started ==== " << std::endl;
	}

	void startHandlingClients() 
	{
		while(1) 
		{
			performPoll();

			for (unsigned i = 0; i < m_pollfds.size(); ++i) {

				if ((m_pollfds.at(i).fd == m_serverSocket) && (m_pollfds.at(i).revents & POLLIN)) 
				{
					acceptNewConnection();
					break;
				}

				if ((m_pollfds.at(i).fd != m_serverSocket) && (m_pollfds.at(i).revents & (POLLIN | POLLOUT))) 
				{
					Client& client = m_clients.at(i - 1);
					int operationStatus = 0;

					if (client.getDataFromClient() == "") {
						operationStatus = SSLDataReader::read(client);
					}

					if (client.getDataFromClient().find("\r\n\r\n") != std::string::npos) {
						/// TODO: change to parsing
						client.createHttpRequest();
					}

					if (client.getDataFromClient() != "" && client.getHttpRequest().size() > 0) {
						operationStatus = SSLDataWriter::write(client);

						// TODO: add better name
						client.clearDataFromClient();
					}

					int errors = SSL_get_error(client.ssl, operationStatus);
					std::cout << errors << std::endl;
					if (errors == SSL_ERROR_WANT_ACCEPT) {
						std::cout << "Accept" << std::endl;
					}
					if (errors == SSL_ERROR_WANT_CONNECT) {
						std::cout << "Connect" << std::endl;
					}
					if (errors == SSL_ERROR_SSL) {
						std::cout << "Problem" << std::endl;
					}

					if (errors == SSL_ERROR_WANT_READ) {
						m_pollfds.at(i).events = POLLIN;
					}
					else if (errors == SSL_ERROR_WANT_WRITE) {
						m_pollfds.at(i).events = POLLOUT;
					}

				}
			}
		}
	}

private:

	void prepareServerAddr(int port) 
	{
		memset(&m_serverAddr, 0, sizeof(m_serverAddr));
		m_serverAddr.sin_family = AF_INET;
		m_serverAddr.sin_port = htons(port);
		m_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}

	void prepareServerSocket() 
	{
		m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		exitOnError(m_serverSocket, "Unable to create socket");
	}

	void setSocketToNonBlocking(int socketToSet)
	{
		int flags = fcntl(socketToSet, F_GETFL);
		int operationStatus = fcntl(socketToSet, F_SETFL, O_NONBLOCK | flags);
		exitOnError(operationStatus, "fcntl() function failed");
	}

	void setSocketOptions() 
	{
		constexpr int one = 1;
		int operationStatus = setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
		exitOnError(operationStatus, "Unable to set socket options");
	}

	void performBind()
	{
		int operationStatus = bind(m_serverSocket, (sockaddr*)&m_serverAddr, sizeof(m_serverAddr));
		exitOnError(operationStatus, "Unable to bind");
	}

	void performListen() 
	{
		int operationStatus = listen(m_serverSocket, 1);
		exitOnError(operationStatus, "Unable to listen");
	}

	void prepareServerPollFD()
	{
		m_pollfds.push_back({m_serverSocket, POLLIN, 0});
	}

	void exitOnError(int operationStatus, const std::string& errorMessage)
	{
		if (operationStatus < 0) {
			perror(errorMessage.c_str());
			exit(EXIT_FAILURE);
		}
	}








	void performPoll()
	{
		int operationStatus = poll(m_pollfds.data(), m_pollfds.size(), -1);
		exitOnError(operationStatus, "poll() function failed");
	}

	void acceptNewConnection()
	{
		sockaddr_in addr;
		uint len = sizeof(addr);

		int clientSocket = accept(m_serverSocket, (sockaddr*)&addr, &len);
		exitOnError(clientSocket, "accept() function failed");

		SSL* newSsl = SSL_new(m_ctx);
		SSL_set_fd(newSsl, clientSocket);
		SSL_set_accept_state(newSsl);

		setSocketToNonBlocking(clientSocket);

		Client newClient(newSsl, addr);

		m_clients.push_back(newClient);
		m_pollfds.push_back({clientSocket, POLLIN | POLLOUT, 0});

		std::cout << " + New connection accepted + " << std::endl;
	}



private:

	int m_serverSocket;
	sockaddr_in m_serverAddr;
	SSL_CTX* m_ctx;

	std::vector<pollfd> m_pollfds;
	std::vector<Client> m_clients;

	static constexpr unsigned BUFFER_SIZE = 50;

};


#endif // Server_hpp