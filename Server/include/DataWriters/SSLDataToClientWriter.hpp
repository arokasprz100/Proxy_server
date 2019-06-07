/**
*	@file SSLDataToClientWriter.hpp
*	@brief This file contains SSLDataToClientWriter class definition that implements writing data to client when the connection is over SSL.
*/


#ifndef SSLDataToClientWriter_hpp
#define SSLDataToClientWriter_hpp

#include "../LogSystem/LogSystem.hpp"
#include "../Client.hpp"

/**
*	@class SSLDataToClientWriter
*/
class SSLDataToClientWriter final
{
public:
	/**
	*	This function implements writing data to client when the connection is over SSL.
	*	@param client A reference to a Client object from server's std::vector of Client objects.
	*	@see Client
	*	@returns Value returned by OpenSSL's SSL_write call.
	*/
	static int write(Client& client);

};

#endif // SSLDataToClientWriter_hpp