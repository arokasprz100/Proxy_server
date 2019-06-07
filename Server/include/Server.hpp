/**
*	@file Server.hpp
*	@brief This file contains the definition of the Server class that implements the server's functions.
*/

/**
*	@mainpage Proxy %Server
*	This is supposed to be on the mainpage.
*	@section installation_section Installation
*	To be able to use this software you need to compile it frist. To do so you first need to navigate to the Server folder.
*	Then you need to run: @code cmake CMakeLists.txt @endcode
*	This command will generate a proper Makefile along other files to compile the project.
*	Next just type: @code make @endcode
*	@warning To do this you need cmake version at least 3.9.6 as well as make installed on top of all the other libraries mentioned in the About section.
*	@section usage_section Usage
*	It's worth noting that the generated Makefile will have an additional target, namely, running:
*	@code make doc_doxygen @endcode
*	Will produce this documentation! Be aware that this doesnt check if the current one is up-to-date and will produce it from scratch.
*	Produced documentation is provided in HTML and LaTeX.
*	Compiled this program has multiple options of being run.
*	@li -h [--help]           To see the help page for this program
*	@li -e [--encrypted]      Use encrypted connection between client and proxy
*	@li -u [ --unencrypted]   Use unencrypted connection between client and proxy, this option is selected by default
*	@li -s [ --silent ]       Do not display log messages, this option is selected by defualt
*	@li -v [ --verbose ]      Display log messages to console
*	@li -m [ --menu ]         Display interactive settings menu. Disabled by default.
*	@section about_section About
*	Compilation of this program requires you to have some libraries installed
*	@li [Boost].multiprecision
*	@li [Boost].program_options
*	
*	On top of that a compiler supporting C++17's fold expressions and parameter pack is needed, such as g++ version 8.2.
*	If that is not your default compiler you can specify a flag to cmake to set it manually.
*	@code cmake -DCMAKE_CXX_COMPILER=g++-8.2 CMakeLists.txt @endcode
*	@authors <a href="https://github.com/dz1domin">Dominik Dziuba</a>, <a href="https://github.com/arokasprz100">Arkadiusz Kasprzak</a>, <a href="https://github.com/Robin-Bjornsdotter">Michalina Oleksy</a>
*/

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
#include <memory>

#include "Initializers/SSLInitializer.hpp"
#include "Initializers/ServerInitializer.hpp"

#include "HttpRequestParser/IPAndPortExtractor.hpp"
#include "HostConnector.hpp"

#include "Client.hpp"
#include "ConnectionType.hpp"

#include "HttpRequestParser/RequestURIParser.hpp"
#include "HttpRequestParser/HttpRequestParser.hpp"

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

#include "HttpResponseFactory.hpp"

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
			closeMarkedConnections();

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

				auto clientConnectionPollFD = getPollFD(client.getClientSocket());
				auto serverConnectionPollFD = getPollFD(client.getServerSocket());

				// connection start
				if (clientConnectionPollFD->revents & (POLLIN | POLLOUT))
				{
					if (client.getConnectionType() == ConnectionType::UNDEFINED) {	
						operationStatus = unencryptedDataFromClientReader(client);
						operationStatusHandler(client, operationStatus, clientConnectionPollFD);
					}
				}

				// only encrypted data
				if (client.getConnectionType() == ConnectionType::ENCRYPTED) {

					// receive data from client
					if (clientConnectionPollFD->revents & (POLLIN | POLLOUT)) {
						operationStatus = encryptedDataFromClientReader(client);
						operationStatusHandler(client, operationStatus, clientConnectionPollFD);
						serverConnectionPollFD->events = POLLOUT | POLLIN;
					}
				}

				// only unencrypted data
				if (client.getConnectionType() == ConnectionType::PLAIN_TEXT) {

					// receive data from client
					if (clientConnectionPollFD->revents & (POLLIN | POLLOUT)) {
						operationStatus = unencryptedDataFromClientReader(client);
						operationStatusHandler(client, operationStatus, clientConnectionPollFD);
						serverConnectionPollFD->events = POLLOUT | POLLIN;
					}
				}
					

				if (clientConnectionPollFD->revents & (POLLIN | POLLOUT) && client.getConnectionType() != ConnectionType::ENCRYPTED)
				{

					try 
					{

						// if full http request present
						const char *crlf = "\r\n\r\n";
						std::vector<char>& dataFromClient = client.getDataFromClient();

						auto endOfRequestPosition = std::search(dataFromClient.begin(), dataFromClient.end(), crlf, crlf + strlen(crlf));

						// if full http request present
						if (endOfRequestPosition != dataFromClient.end()) 
						{

							std::shared_ptr<HttpRequest> request (HttpRequestParser::parseHttpRequest(dataFromClient));
							if (request->getHttpMethod() == Method::ERROR) {
								throw std::runtime_error("502");
							}

							std::string resourcePath = request->getResourcePath();

							// if connect method received, make connection to server
							if (request->getHttpMethod() == Method::CONNECT) {

								if (client.getConnectionType() == ConnectionType::UNDEFINED) 
								{
									auto [ip, port] = IPAndPortExtractor::extractIPAddressAndPortNumberFromURI(resourcePath);
									auto pollFD = HostConnector::connectToHost(client, ip, port);
									m_pollfds.push_back(pollFD);
									client.setConnectionType(ConnectionType::ENCRYPTED);
									clientConnectionPollFD = getPollFD(client.getClientSocket());
									serverConnectionPollFD = getPollFD(client.getServerSocket());
								}

								LogSystem::logMessage("Full CONNECT request received", "RECEIVED", std::to_string(client.getID()));
								std::vector<char> parsedRequest = request->getFullRequest();

								if (request->getHeadersSize() > 8000) {
									throw std::runtime_error("413");
								} else {
									client.m_httpResponseFromServer = HttpResponseFactory::getResponseByCode("200");

									operationStatus = dataToClientWriter(client);
									operationStatusHandler(client, operationStatus, clientConnectionPollFD);
									serverConnectionPollFD->events = POLLIN | POLLOUT;
								}

								dataFromClient.erase(dataFromClient.begin(), dataFromClient.begin() + parsedRequest.size());
							}
							else 
							{
								RequestURI requestURI = RequestURIParser::parse(resourcePath);

								if (request->getProtocolFromResourcePath() != "http") {
									throw std::runtime_error("501");
								}
								else {

									if (client.getConnectionType() == ConnectionType::UNDEFINED) 
									{
										auto [ip, port] = IPAndPortExtractor::extractIPAddressAndPortNumberFromURI(requestURI.nonLocalPart);
										auto pollFD = HostConnector::connectToHost(client, ip, port);
										m_pollfds.push_back(pollFD);
										client.setConnectionType(ConnectionType::PLAIN_TEXT);
										clientConnectionPollFD = getPollFD(client.getClientSocket());
										serverConnectionPollFD = getPollFD(client.getServerSocket());
									}

									// if request complete
									if (request->isRequestComplete()) {

										request->changeNumberSequenceInBody(serverSettings.getConnectionData26Numbers());
										std::vector<char> parsedRequest = request->getFullRequest();
										int originalRequestSize = parsedRequest.size();

										LogSystem::logMessage("Full " + request->getHttpMethodAsString() +" request received", "RECEIVED", std::to_string(client.getID()));

										if (request->getHeadersSize() > 8000) {
											throw std::runtime_error("413");
										} 
										else {
											request->eraseNonLocalPartOfResourcePath();
											parsedRequest = request->getFullRequest();
											client.m_httpRequestFromClient.insert(client.m_httpRequestFromClient.end(),
												parsedRequest.begin(), parsedRequest.end());
										}

										dataFromClient.erase(dataFromClient.begin(), dataFromClient.begin() + originalRequestSize);

										serverConnectionPollFD->events = POLLOUT | POLLIN;
										clientConnectionPollFD->events = POLLOUT | POLLIN;
									}

								}

							}
						}

					} catch(std::runtime_error& e) {
						std::string message = e.what();
						client.m_httpResponseFromServer = HttpResponseFactory::getResponseByCode(message);
						LogSystem::logMessage("Sending: " + message, "ERROR", std::to_string(client.getID()));
						dataToClientWriter(client);
						client.clearDataFromClient();
						connectionsToClose.push_back(client.getClientSocket());
						continue;
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
				if(clientUptime.count() > serverSettings.getTimeoutLimit())
				{
					LogSystem::logMessage("Client uptime: " + std::to_string(clientUptime.count()) + "s", "CLOSING", std::to_string(client.getID()));
					connectionsToClose.push_back(client.getClientSocket());
					continue;
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

	void closeMarkedConnections()
	{
		for (unsigned i = 0; i < connectionsToClose.size(); ++i) {
			auto clientPosition = std::find_if(m_clients.begin(), m_clients.end(), [this, &i] (Client& client) {
				return client.getClientSocket() == connectionsToClose[i];
			});
			int clientIndex = std::distance(m_clients.begin(), clientPosition);
			closeConnection(*clientPosition, clientIndex);
		}
		connectionsToClose.clear();
	}

	void closeConnection(Client& client, int clientIndex)
	{
		LogSystem::logMessage("Closing connection", "CLOSING", std::to_string(client.getID()));
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
	std::vector<int> connectionsToClose;


	std::function<int(Client&)> unencryptedDataFromClientReader;
	std::function<int(Client&)> encryptedDataFromClientReader;
	std::function<int(Client&)> dataToClientWriter;
	std::function<void(Client&, int, std::vector<pollfd>::iterator)> operationStatusHandler;
	std::function<std::tuple<Client, pollfd>(int, SSL_CTX*)> clientAcceptor;
};


#endif // Server_hpp