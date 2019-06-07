/**
*	@file SSLInitializer.hpp
*	@brief This file contains definitions of functions that initialize important OpenSSL variables and call required OpenSSL functions.
*/

#ifndef SSLInitializer_hpp
#define SSLInitializer_hpp

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <string>

/**
*	@class SSLInitializer
*/
class SSLInitializer final
{
public:
	/**
	*	This member function calls required OpenSSL functions and sets up OpenSSL context using which the server will communicate with client if the client to proxy connection is over SSL.
	*	@param certificateFilePath	A reference to string object containing the path to the server's certificate file.
	* 	@param privateKeyFilePath A reference to string object containing the path to the server's private key file.
	*	@returns Pointer to context created by OpenSSL functions.
	*/
	static SSL_CTX* initialize(const std::string& certificateFilePath, const std::string& privateKeyFilePath);

private:

	static void initOpenssl();

	static SSL_CTX* prepareContext();

	static void configureContextKeyAndCert(SSL_CTX * ctx, const std::string& certificateFilePath, const std::string& privateKeyFilePath);
	
};


#endif // SSLInitializer_hpp