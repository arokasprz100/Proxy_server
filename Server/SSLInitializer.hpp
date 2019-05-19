#ifndef SSLInitializer_hpp
#define SSLInitializer_hpp

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "ServerSettings.hpp"

class SSLInitializer final
{
public:

	static SSL_CTX* initializeOpenSSL() {
		initOpenssl();
		SSL_CTX* ctx = prepareContext();
		configureContextKeyAndCert(ctx);
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
		return ctx;
	}

	static void configureContextKeyAndCert(SSL_CTX * ctx)
	{
		SSL_CTX_set_ecdh_auto(ctx, 1);

		if (SSL_CTX_use_certificate_file(ctx, ServerSettings::certificateFilePath, SSL_FILETYPE_PEM) <= 0) {
			ERR_print_errors_fp(stderr);
			exit(EXIT_FAILURE);
		}

		if (SSL_CTX_use_PrivateKey_file(ctx, ServerSettings::privateKeyFilePath, SSL_FILETYPE_PEM) <= 0 ) {
			ERR_print_errors_fp(stderr);
			exit(EXIT_FAILURE);
		}
	}

};


#endif // SSLInitializer_hpp