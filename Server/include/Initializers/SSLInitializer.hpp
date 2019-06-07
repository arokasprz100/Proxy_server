/**
*	@file SSLInitializer.hpp
*	@brief This file contains definitions of functions that initialize important OpenSSL variables and call required OpenSSL functions.
*/

#ifndef SSLInitializer_hpp
#define SSLInitializer_hpp

#include <openssl/ssl.h>
#include <openssl/err.h>

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
	static SSL_CTX* initialize(const std::string& certificateFilePath, const std::string& privateKeyFilePath) {
		initOpenssl();
		SSL_CTX* ctx = prepareContext();
		configureContextKeyAndCert(ctx, certificateFilePath, privateKeyFilePath);
		return ctx;
	}

private:

	static void initOpenssl()
	{
		SSL_load_error_strings();	
		OpenSSL_add_ssl_algorithms();
	}

	static SSL_CTX* prepareContext()
	{
		SSL_CTX * ctx = SSL_CTX_new(SSLv23_server_method());
		if (!ctx) {
			perror("Unable to create SSL context");
			ERR_print_errors_fp(stderr);
			exit(EXIT_FAILURE);
		}
		SSL_CTX_set_mode(ctx, SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);
		return ctx;
	}

	static void configureContextKeyAndCert(SSL_CTX * ctx, const std::string& certificateFilePath, const std::string& privateKeyFilePath)
	{
		SSL_CTX_set_ecdh_auto(ctx, 1);

		if (SSL_CTX_use_certificate_file(ctx, certificateFilePath.c_str(), SSL_FILETYPE_PEM) <= 0) {
			ERR_print_errors_fp(stderr);
			exit(EXIT_FAILURE);
		}

		if (SSL_CTX_use_PrivateKey_file(ctx, privateKeyFilePath.c_str(), SSL_FILETYPE_PEM) <= 0 ) {
			ERR_print_errors_fp(stderr);
			exit(EXIT_FAILURE);
		}
	}

};


#endif // SSLInitializer_hpp