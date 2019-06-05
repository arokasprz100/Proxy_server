#ifndef SLLDataFromClientReader_hpp
#define SSLDataFromClientReader_hpp

class SSLDataFromClientReader
{
public:

	static int readUnencryptedData(Client& client) {
		int operationStatus = 0;
		do {
			char buffor[1000] = {'\0'};
			operationStatus = SSL_read(client.ssl, buffor, 999);
			if (operationStatus > 0) {
				client.refreshTimestamp();
				client.addDataFromClient(std::vector<char>(buffor, buffor + operationStatus));
			}

		} while (SSL_pending(client.ssl));

		return operationStatus;
	}

	static int readEncryptedData(Client& client) {
		int operationStatus = 0;
		do {
			char buffor[1000] = {'\0'};
			operationStatus = SSL_read(client.ssl, buffor, 999);
			if (operationStatus > 0) {
				client.refreshTimestamp();
				client.m_httpRequestFromClient.insert(client.m_httpRequestFromClient.end(), buffor, buffor + operationStatus);
			}

		} while (SSL_pending(client.ssl));

		return operationStatus;
	}
};

#endif // SSLDataFromClientReader_hpp