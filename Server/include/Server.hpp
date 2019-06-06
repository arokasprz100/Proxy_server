#ifndef Server_hpp
#define Server_hpp

#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <poll.h>

#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

#include "Initializers/SSLInitializer.hpp"
#include "Initializers/ServerInitializer.hpp"

#include "IPAndPortExtractor.hpp"
#include "HostConnector.hpp"

#include "Client.hpp"
#include "ConnectionType.hpp"

#include "RequestURIParser.hpp"
#include "HttpRequestParserDummy.hpp"

#include "LogSystem/LogSystem.hpp"

#include "ClientConnectionType.hpp"

#include "ClientAcceptors/ClientAcceptorChooser.hpp"
#include "DataReaders/DataFromClientReaderChooser.hpp"
#include "DataWriters/DataToClientWriterChooser.hpp"
#include "OperationStatusHandlers/OperationStatusHandlerChooser.hpp"

#include "Disconnectors/HostDisconnector.hpp"
#include "Disconnectors/ClientDisconnector.hpp"

#include "DataReaders/DataFromServerReader.hpp"
#include "DataWriters/DataToServerWriter.hpp"

#include "ServerSettings/ServerSettings.hpp"

/*
TODO: integrate http request parser (and protocol and size constraints)
TODO: integrate algorithm
TODO: refactor
TODO: write doxs
*/

class Server
{
public:

	Server(int port, ClientConnectionType clientConnectionType, const ServerSettings& serverSettings) : 
		serverSettings(serverSettings),
		unencryptedDataFromClientReader(DataFromClientReaderChooser::chooseUnencryptedDataFromClientReader(clientConnectionType)),
		encryptedDataFromClientReader(DataFromClientReaderChooser::chooseEncryptedDataFromClientReader(clientConnectionType)),
		dataToClientWriter(DataToClientWriterChooser::chooseDataToClientWriter(clientConnectionType)),
		operationStatusHandler(OperationStatusHandlerChooser::chooseOperationStatusHandler(clientConnectionType)),
		clientAcceptor(ClientAcceptorChooser::chooseClientAcceptor(clientConnectionType))
	{	
		LogSystem::logMessage("Starting new server.", "START");

		auto[serverSocket, serverAddr] = ServerInitializer::initialize(port, m_pollfds);
		m_serverSocket = serverSocket;
		m_serverAddr = serverAddr;

		if (clientConnectionType == ClientConnectionType::ENCRYPTED) {
			m_ctx = SSLInitializer::initialize(serverSettings.getCertificateFilePath(), serverSettings.getPrivateKeyFilePath());
		}

		LogSystem::logMessage("Server started.", "START");
	}

	~Server() {
		for (long unsigned int clientIndex = 0; clientIndex < m_clients.size(); ++clientIndex) {
			auto& client = m_clients[clientIndex];
			closeConnection(client, clientIndex);
		}

		LogSystem::logMessage("Closing server.", "CLOSE");
	}

	void startHandlingClients() 
	{
		while(1) 
		{	
			performPoll();

			if (m_pollfds.at(0).revents & POLLIN) {
				if (m_clients.size() <= serverSettings.getMaxNumberOfConnections()) {
					acceptNewConnection();
				}
				else {
					LogSystem::logMessage("Maximum number of connections reached.", "ACCEPTING");
				}
			}

			for (long unsigned int clientIndex = 0; clientIndex < m_clients.size(); ++clientIndex) 
			{
				auto& client = m_clients[clientIndex];
				int operationStatus = 0;

				auto clientConnectionPollFD = getPollFD(client.clientSocket);
				auto serverConnectionPollFD = getPollFD(client.serverSocket);

				// odbieranie niezaszyfrowanych danych - poczatek polaczenia
				if (clientConnectionPollFD->revents & (POLLIN | POLLOUT))
				{
					if (client.getConnectionType() == ConnectionType::UNDEFINED) {	
						operationStatus = unencryptedDataFromClientReader(client);
						operationStatusHandler(client, operationStatus, clientConnectionPollFD);
					}
				}

				// sekcja odpowiedzialna tylko za forwardowanie polaczenia tls
				if (client.getConnectionType() == ConnectionType::ENCRYPTED) {

					// odbieranie danych od klienta
					if (clientConnectionPollFD->revents & (POLLIN | POLLOUT)) {
						operationStatus = encryptedDataFromClientReader(client);
						operationStatusHandler(client, operationStatus, clientConnectionPollFD);
						serverConnectionPollFD->events = POLLOUT | POLLIN;
					}
				}

				// tylko nieszyfrowane dane
				if (client.getConnectionType() == ConnectionType::PLAIN_TEXT) {

					// odbieranie danych od klienta
					if (clientConnectionPollFD->revents & (POLLIN | POLLOUT)) {
						operationStatus = unencryptedDataFromClientReader(client);
						operationStatusHandler(client, operationStatus, clientConnectionPollFD);
						serverConnectionPollFD->events = POLLOUT | POLLIN;
					}
				}
					

				if (clientConnectionPollFD->revents & (POLLIN | POLLOUT) && client.getConnectionType() != ConnectionType::ENCRYPTED)
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

							if (client.getConnectionType() == ConnectionType::UNDEFINED) {
								auto [ip, port] = IPAndPortExtractor::extractIPAddressAndPortNumberFromURI(request.hostname);
								auto pollFD = HostConnector::connectToHost(client, ip, port);
								m_pollfds.push_back(pollFD);
								client.setConnectionType(ConnectionType::ENCRYPTED);
								clientConnectionPollFD = getPollFD(client.clientSocket);
								serverConnectionPollFD = getPollFD(client.serverSocket);
							}

							LogSystem::logMessage("Full CONNECT request received", "RECEIVED", std::to_string(client.getID()));

							// set 200 confirming that it worked
							std::string tempHttpResp = std::string("HTTP/1.1 200 Connection Established\r\n") + 
								"Reason: Connection Established\r\n" + 
								"Proxy-Connection: Keep-Alive\r\n" + 
								"Connection: Keep-Alive\r\n\r\n";
							client.m_httpResponseFromServer = std::vector<char>(tempHttpResp.begin(), tempHttpResp.end());

							operationStatus = dataToClientWriter(client);
							operationStatusHandler(client, operationStatus, clientConnectionPollFD);
							serverConnectionPollFD->events = POLLIN | POLLOUT;

							client.clearDataFromClient();

						}
						else 
						{
							RequestURI requestURI = RequestURIParser::parse(request.hostname);
							if (client.getConnectionType() == ConnectionType::UNDEFINED) {

								auto [ip, port] = IPAndPortExtractor::extractIPAddressAndPortNumberFromURI(requestURI.nonLocalPart);
								auto pollFD = HostConnector::connectToHost(client, ip, port);
								m_pollfds.push_back(pollFD);
								client.setConnectionType(ConnectionType::PLAIN_TEXT);
								clientConnectionPollFD = getPollFD(client.clientSocket);
								serverConnectionPollFD = getPollFD(client.serverSocket);
							}

							LogSystem::logMessage("Full " + request.method + " request received", "RECEIVED", std::to_string(client.getID()));

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

							serverConnectionPollFD->events = POLLOUT | POLLIN;
							clientConnectionPollFD->events = POLLOUT | POLLIN;

						}
					}
				}
				
				
				if (client.getConnectionType() != ConnectionType::UNDEFINED) {

					// send data to server
					if ((serverConnectionPollFD->revents & POLLOUT) && client.m_httpRequestFromClient.size() > 0) {
						DataToServerWriter::write(client);
						serverConnectionPollFD->events = POLLIN | POLLOUT;
					}

					// receive data from server
					if (serverConnectionPollFD->revents & POLLIN) {
						DataFromServerReader::read(client);
						serverConnectionPollFD->events = POLLOUT | POLLIN;
						clientConnectionPollFD->events = POLLOUT | POLLIN;					
					}

					if (clientConnectionPollFD->revents & (POLLOUT | POLLIN)) {

						// send data to client
						if (client.m_httpResponseFromServer.size() > 0) {
							operationStatus = dataToClientWriter(client);
							operationStatusHandler(client, operationStatus, clientConnectionPollFD);
							serverConnectionPollFD->events = POLLOUT | POLLIN;
						}
					}
				
				}

				// close timed-out connection
				auto clientUptime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - client.getCurrentTimestamp());
				if(clientUptime.count() > 10.0)
				{
					LogSystem::logMessage("Client uptime: " + std::to_string(clientUptime.count()) + "s", "CLOSING", std::to_string(client.getID()));
					closeConnection(client, clientIndex);
					break;
				}

			}
		}
	}

private: // methods

	std::vector<pollfd>::iterator getPollFD(int socketNumber) {
		return std::find_if(m_pollfds.begin(), m_pollfds.end(), [&socketNumber](auto& pollFD) {
			return socketNumber == pollFD.fd;
		});
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
		auto[client, pollFD] = clientAcceptor(m_serverSocket, m_ctx);
		m_pollfds.push_back(pollFD);
		m_clients.push_back(client);
		LogSystem::logMessage("New connection accepted", "ACCEPTING", std::to_string(client.getID()));
	}

	void closeConnection(Client& client, int clientIndex)
	{
		ClientDisconnector::disconnect(client, m_pollfds);
		HostDisconnector::disconnect(client, m_pollfds);
		m_clients.erase(m_clients.begin() + clientIndex);
	}

private: // state

	int m_serverSocket;
	sockaddr_in m_serverAddr;
	SSL_CTX* m_ctx;

	const ServerSettings& serverSettings; // TODO: make unique_ptr

	std::vector<pollfd> m_pollfds;
	std::vector<Client> m_clients;

	std::function<int(Client&)> unencryptedDataFromClientReader;
	std::function<int(Client&)> encryptedDataFromClientReader;
	std::function<int(Client&)> dataToClientWriter;
	std::function<void(Client&, int, std::vector<pollfd>::iterator)> operationStatusHandler;
	std::function<std::tuple<Client, pollfd>(int, SSL_CTX*)> clientAcceptor;
};


#endif // Server_hpp