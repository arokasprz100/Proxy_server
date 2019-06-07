/**
*	@file DataFromServerReader.hpp
*	@brief This file contains definition of DataFromServerReader implementing reading data received from destination server.
*/

#ifndef DataFromServerReader_hpp
#define DataFromServerReader_hpp

#include "../LogSystem/LogSystem.hpp"
#include "../Client.hpp"

/**
*	@class DataFromServerReader
*/
class DataFromServerReader
{
public:
	/**
	*	This member function implements reading data from destination server.
	*	@param client A reference to a Client object from server's std::vector of Client objects.
	*	@see Client
	*	@returns Value returned by recv() call.
	*/
	static int read(Client& client);
};

#endif // DataFromServerReader_hpp