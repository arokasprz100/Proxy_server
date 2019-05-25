#ifndef SLLDataFromClientReader_hpp
#define SSLDataFromClientReader_hpp

class SSLDataFromClientReader
{
public:

	static int readUnencryptedDataFromClient(Client& client) {
		char buffor[3000] = {'\0'};
		int operationStatus = SSL_read(client.ssl, buffor, 2999);
		if (operationStatus > 0) {
			client.addDataFromClient(std::vector<char>(buffor, buffor + operationStatus));
		}
		while (SSL_pending(client.ssl)) {
			char buffor2[3000] = {'\0'};
			operationStatus = SSL_read(client.ssl, buffor2, 2999);
			if (operationStatus > 0) {
				client.addDataFromClient(std::vector<char>(buffor2, buffor2 + operationStatus));
			}
		}
		// std::cout << "[RECV FROM CLIENT STATUS] " << operationStatus << std::endl;
		return operationStatus;
	}

	static int readEncryptedDataFromClient(Client& client) {
		char buffor[3000] = {'\0'};
		int operationStatus = SSL_read(client.ssl, buffor, 2999);
		if (operationStatus > 0) {
			client.m_httpRequestFromClient.insert(client.m_httpRequestFromClient.end(), buffor, buffor + operationStatus);
		}
		while (SSL_pending(client.ssl)) {
			char buffor2[3000] = {'\0'};
			operationStatus = SSL_read(client.ssl, buffor2, 2999);
			if (operationStatus > 0) {
				client.m_httpRequestFromClient.insert(client.m_httpRequestFromClient.end(), buffor, buffor + operationStatus);
			}
		}
		return operationStatus;
	}
};

#endif // SSLDataFromClientReader_hpp