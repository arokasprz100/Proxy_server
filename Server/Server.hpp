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
#include <algorithm>

#include "SSLInitializer.hpp"
#include "ServerInitializer.hpp"

#include "IPAndPortExtractor.hpp"
#include "HostConnector.hpp"

#include "Client.hpp"
#include "ConnectionType.hpp"

#include "RequestURIParser.hpp"
#include "HttpRequestParserDummy.hpp"

#include "DataReaders/DataFromClientReader.hpp"
#include "DataReaders/SSLDataFromClientReader.hpp"
#include "DataReaders/DataFromServerReader.hpp"

#include "DataWriters/DataToServerWriter.hpp"
#include "DataWriters/DataToClientWriter.hpp"
#include "DataWriters/SSLDataToClientWriter.hpp"

#include "LogSystem/LogSystem.hpp"

class Server
{
public:

	Server(int port)
	{	
		std::cout << " ==== Starting new server ==== " << std::endl;

		auto[serverSocket, serverAddr] = ServerInitializer::initialize(port, m_pollfds);
		m_serverSocket = serverSocket;
		m_serverAddr = serverAddr;

		m_ctx = SSLInitializer::initialize();

		std::cout << " ==== Server started ==== " << std::endl;
	}

	void startHandlingClients() 
	{
		while(1) 
		{	
			performPoll(m_pollfds);

			if (m_pollfds.at(0).revents & POLLIN) {
				acceptNewConnection();
			}

			for (auto& client : m_clients) 
			{
				int operationStatus = 0;

				// odbieranie niezaszyfrowanych danych - poczatek polaczenia
				if (client.clientConnectionPollFD->revents & (POLLIN | POLLOUT))
				{
					if (client.connectionType == ConnectionType::UNDEFINED) {	
						operationStatus = SSLDataFromClientReader::readUnencryptedDataFromClient(client);
						handleOperationStatus(client, operationStatus);
					}
				}

				// sekcja odpowiedzialna tylko za forwardowanie polaczenia tls
				if (client.connectionType == ConnectionType::ENCRYPTED) {

					// odbieranie danych od klienta
					if (client.clientConnectionPollFD->revents & (POLLIN | POLLOUT)) {
						operationStatus = SSLDataFromClientReader::readEncryptedDataFromClient(client);
						handleOperationStatus(client, operationStatus);

					}
				}

				// tylko nieszyfrowane dane
				if (client.connectionType == ConnectionType::PLAIN_TEXT) {

					// odbieranie danych od klienta
					if (client.clientConnectionPollFD->revents & (POLLIN | POLLOUT)) {
						operationStatus = SSLDataFromClientReader::readUnencryptedDataFromClient(client);
						handleOperationStatus(client, operationStatus);
					}
				}
					

				if (client.clientConnectionPollFD->revents & (POLLIN | POLLOUT) && client.connectionType != ConnectionType::ENCRYPTED)
				{

					// if full http request present
					const char *crlf2 = "\r\n\r\n";
					std::vector<char> dataFromClient = client.getDataFromClient();

					auto endOfRequestPosition = std::search(dataFromClient.begin(), dataFromClient.end(), crlf2, crlf2 + strlen(crlf2));

					if (endOfRequestPosition != dataFromClient.end()) 
					{
						HttpRequest request = HttpRequestParserDummy::parse(
							std::string(dataFromClient.begin(), 
								dataFromClient.end()));

						// if connect method received, make connection to server
						if (request.method == "CONNECT") {
							LogSystem::logMessage("Full CONNECT request received", "RECEIVED", std::to_string(client.id));

							auto [ip, port] = IPAndPortExtractor::extractIPAddressAndPortNumberFromURI(request.hostname);
							HostConnector::connectToHost(client, ip, port, *this);

							client.connectionType = ConnectionType::ENCRYPTED;

							// set 200 confirming that it worked
							std::string tempHttpResp = std::string("HTTP/1.1 200 Connection Established\r\n") + 
								"Reason: Connection Established\r\n" + 
								"Proxy-Connection: Keep-Alive\r\n" + 
								"Connection: Keep-Alive\r\n\r\n";
							client.m_httpResponseFromServer = std::vector<char>(tempHttpResp.begin(), tempHttpResp.end());

							operationStatus = SSLDataToClientWriter::writeDataToClient(client);
							handleOperationStatus(client, operationStatus);

							client.clientConnectionPollFD->events = POLLIN | POLLOUT;
							client.clearDataFromClient();

						}
						else 
						{
							LogSystem::logMessage("Full " + request.method + " request received", "RECEIVED", std::to_string(client.id));

							RequestURI requestURI = RequestURIParser::parse(request.hostname);
							if (client.connectionType == ConnectionType::UNDEFINED) {

								auto [ip, port] = IPAndPortExtractor::extractIPAddressAndPortNumberFromURI(requestURI.nonLocalPart);
								HostConnector::connectToHost(client, ip, port, *this);
								client.connectionType = ConnectionType::PLAIN_TEXT;
							}

							std::vector<char> requestText = client.getDataFromClient();
							auto afterMethod = std::find(requestText.begin(), requestText.end(), ' ');

							const char* nonLocal = requestURI.nonLocalPart.c_str();
							auto afterNonLocal = std::search(requestText.begin(), 
								requestText.end(), 
								nonLocal, nonLocal + strlen(nonLocal)) + strlen(nonLocal);

							requestText.erase(afterMethod + 1, afterNonLocal);

							client.m_httpRequestFromClient.insert(client.m_httpRequestFromClient.end(),
								requestText.begin(), requestText.end());

							auto endOfRequestPosition2 = std::search(client.m_dataFromClient.begin(), client.m_dataFromClient.end(), 
								crlf2, crlf2 + strlen(crlf2));

							client.m_dataFromClient.erase(client.m_dataFromClient.begin(), endOfRequestPosition2 + 4);

							client.serverConnectionPollFD->events = POLLOUT | POLLIN;

						}
					}


				}
				
				
				if (client.connectionType != ConnectionType::UNDEFINED) {

					// send data to server
					if ((client.serverConnectionPollFD->revents & POLLOUT) && client.m_httpRequestFromClient.size() > 0) {
						DataToServerWriter::writeDataToServer(client);
					}

					// receive data from server
					if (client.serverConnectionPollFD->revents & POLLIN) {
						DataFromServerReader::readDataFromServer(client);						
					}

					if (client.clientConnectionPollFD->revents & (POLLOUT | POLLIN)) {

						// send data to client
						if (client.m_httpResponseFromServer.size() > 0) {
							operationStatus = SSLDataToClientWriter::writeDataToClient(client);
							handleOperationStatus(client, operationStatus);
						}
					}
				
				}

			}
		}
	}

	void handleOperationStatus(Client& client, int operationStatus) {
		if (operationStatus != 0) {
			int errors = SSL_get_error(client.ssl, operationStatus);
			if (errors == SSL_ERROR_WANT_READ) {
				client.clientConnectionPollFD->events |= POLLIN;
			}
			else if (errors == SSL_ERROR_WANT_WRITE) {
				client.clientConnectionPollFD->events |= POLLOUT;
			}
		}
	}

	void setSocketToNonBlocking(int socketToSet)
	{
		int flags = fcntl(socketToSet, F_GETFL);
		int operationStatus = fcntl(socketToSet, F_SETFL, O_NONBLOCK | flags);
		exitOnError(operationStatus, "fcntl() function failed");
	}

	void exitOnError(int operationStatus, const std::string& errorMessage)
	{
		if (operationStatus < 0) {
			perror(errorMessage.c_str());
			exit(EXIT_FAILURE);
		}
	}

	void performPoll(std::vector<pollfd>& pollfds)
	{
		int operationStatus = poll(pollfds.data(), pollfds.size(), -1);
		exitOnError(operationStatus, "poll() function failed");
	}

	void acceptNewConnection()
	{
		static int nextId = 0;

		sockaddr_in addr;
		uint len = sizeof(addr);

		int clientSocket = accept(m_serverSocket, (sockaddr*)&addr, &len);
		exitOnError(clientSocket, "accept() function failed");
		
		SSL* newSsl = SSL_new(m_ctx);
		SSL_set_fd(newSsl, clientSocket);
		SSL_set_accept_state(newSsl);
		
		setSocketToNonBlocking(clientSocket);

		Client newClient(newSsl, addr);
		newClient.clientSocket = clientSocket;

		m_pollfds.push_back({clientSocket, POLLIN | POLLOUT, 0});

		newClient.clientConnectionPollFD = &(m_pollfds.back());
		newClient.serverConnectionPollFD = nullptr;
		newClient.id = nextId;
		nextId++;

		m_clients.push_back(newClient);

		std::cout << " + New connection accepted + " << std::endl;
	}

// TODO: make private
public:

	int m_serverSocket;
	sockaddr_in m_serverAddr;
	SSL_CTX* m_ctx;

	std::vector<pollfd> m_pollfds;

	std::vector<Client> m_clients;

};


#endif // Server_hpp