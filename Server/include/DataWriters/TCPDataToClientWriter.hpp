/**
*	@file TCPDataToClientWriter.hpp
*	@brief This file contains TCPDataToClientWriter class definition that implements writing data to client when the connection is over plain HTTP.
*/

#ifndef TCPDataToClientWriter_hpp
#define TCPDataToClientWriter_hpp

#include "../LogSystem/LogSystem.hpp"
#include "../Client.hpp"

/**
*	@class TCPDataToClientWriter
*/
class TCPDataToClientWriter
{
public:
	/**
	*	This function implements writing data to client when the connection is over plain HTTP.
	*	@param client A reference to a Client object from server's std::vector of Client objects.
	*	@see Client
	*	@returns Value returned by send() call.
	*/
	static int write (Client& client);

};

#endif // TCPDataToClientWriter_hpp