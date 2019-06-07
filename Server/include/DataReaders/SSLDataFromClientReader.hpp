/**
*	@file SSLDataFromClientReader.hpp
*	@brief 
*/

#ifndef SSLDataFromClientReader_hpp
#define SSLDataFromClientReader_hpp

/**
*	@class SSLDataFromClientReader
*/
class SSLDataFromClientReader
{
public:
	/**
	*	This member fuction implemets reading unencrypted data from client when the connection to proxy is over SSL.
	*	@param client A reference to a Client object from server's std::vector of Client objects.
	*	@see Client
	*	@returns Value returned by OpenSSL's SSL_read() call.
	*/
	static int readUnencryptedData(Client& client) {
		int operationStatus = 0;
		do {
			char buffor[1000] = {'\0'};
			operationStatus = SSL_read(client.getSSL(), buffor, 999);
			if (operationStatus > 0) {
				client.refreshTimestamp();
				client.addDataFromClient(std::vector<char>(buffor, buffor + operationStatus));
			}

		} while (SSL_pending(client.getSSL()));

		return operationStatus;
	}

	/**
	*	This member fuction implemets reading encrypted data from client when the connection to proxy is over SSL.
	*	@param client A reference to a Client object from server's std::vector of Client objects.
	*	@see Client
	*	@returns Value returned by OpenSSL's SSL_read() call.
	*/
	static int readEncryptedData(Client& client) {
		int operationStatus = 0;
		do {
			char buffor[1000] = {'\0'};
			operationStatus = SSL_read(client.getSSL(), buffor, 999);
			if (operationStatus > 0) {
				client.refreshTimestamp();
				client.m_httpRequestFromClient.insert(client.m_httpRequestFromClient.end(), buffor, buffor + operationStatus);
			}

		} while (SSL_pending(client.getSSL()));

		return operationStatus;
	}
};

#endif // SSLDataFromClientReader_hpp