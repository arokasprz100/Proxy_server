/**
*	@file TCPDataFromClientReader.hpp
*	@brief This file contains definition of TCPDataFromClientReader that contains methods implementing reading unencrypted and encrypted data from client.
*/

#ifndef TCPDataFromClientReader_hpp
#define TCPDataFromClientReader_hpp

#include "../Client.hpp"
#include "../LogSystem/LogSystem.hpp"

/**
*	@class TCPDataFromClientReader
*/
class TCPDataFromClientReader
{
public:
	/**
	*	This member function implements reading unecrypted data from client.
	*	@param client A reference to a Client object from server's std::vector of Client objects.
	*	@see Client
	*	@returns Value returned by recv() call.
	*/
	static int readUnencryptedData(Client& client);
	
	/**
	*	This member function implements reading ecrypted data from client.
	*	@param client A reference to a Client object from server's std::vector of Client objects.
	*	@see Client
	*	@returns Value returned by recv() call.
	*/
	static int readEncryptedData(Client& client);

};

#endif // TCPDataFromClientReader_hpp