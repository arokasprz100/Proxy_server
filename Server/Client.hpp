#ifndef Client_hpp
#define Client_hpp

#include <iostream>
#include <string>
#include <vector>

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
		m_httpRequest.clear();
	}

	void addDataFromClient(const std::string& dataToAdd) {
		m_dataFromClient += dataToAdd;
	}

	void createHttpRequest() {
		m_httpRequest = m_dataFromClient;
	}

	const std::string getDataFromClient() const {
		return m_dataFromClient;
	}

	const std::string getHttpRequest() const {
		return m_httpRequest;
	}


	SSL* ssl;

private:
	std::string m_dataFromClient;

	sockaddr_in m_clientAddr; 
	std::string m_httpRequest;
};

#endif // Client_hpp