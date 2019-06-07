/**
*	@file DataToServerWriter.hpp
*	@brief This file contains DataToServerWriter class definition that implements writing data to destination server.
*/

#ifndef DataToServerWriter_hpp
#define DataToServerWriter_hpp

#include "../LogSystem/LogSystem.hpp"
#include "../Client.hpp"

/**
*	@class DataToServerWriter
*/
class DataToServerWriter final
{
public:
	/**
	*	This member function implements writing data to destination server.
	*	@param client A reference to a Client object from server's std::vector of Client objects.
	*	@see Client
	*	@returns Value returned by send() call.
	*/
	static int write(Client& client);
	
};

#endif // DataToServerWriter_hpp