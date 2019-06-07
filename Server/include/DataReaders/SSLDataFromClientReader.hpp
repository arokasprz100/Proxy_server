/**
*	@file SSLDataFromClientReader.hpp
*	@brief 
*/

#ifndef SSLDataFromClientReader_hpp
#define SSLDataFromClientReader_hpp

#include "../LogSystem/LogSystem.hpp"
#include "../Client.hpp"

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
	static int readUnencryptedData(Client& client);

	/**
	*	This member fuction implemets reading encrypted data from client when the connection to proxy is over SSL.
	*	@param client A reference to a Client object from server's std::vector of Client objects.
	*	@see Client
	*	@returns Value returned by OpenSSL's SSL_read() call.
	*/
	static int readEncryptedData(Client& client);
};

#endif // SSLDataFromClientReader_hpp