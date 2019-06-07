#include "../../include/Initializers/SSLInitializer.hpp"

SSL_CTX* SSLInitializer::initialize(const std::string& certificateFilePath, const std::string& privateKeyFilePath) 
{
	initOpenssl();
	SSL_CTX* ctx = prepareContext();
	configureContextKeyAndCert(ctx, certificateFilePath, privateKeyFilePath);
	return ctx;
}

void SSLInitializer::initOpenssl()
{
	SSL_load_error_strings();	
	OpenSSL_add_ssl_algorithms();
}

SSL_CTX* SSLInitializer::prepareContext()
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

void SSLInitializer::configureContextKeyAndCert(SSL_CTX * ctx, const std::string& certificateFilePath, const std::string& privateKeyFilePath)
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