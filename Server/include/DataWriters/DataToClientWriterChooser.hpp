/**
*	@file DataToClientWriterChooser.hpp
*	@brief This file contains the definition of DataToClientWriterChooser that handles returning appropriate function according to client's connection to destination server.
*/

#ifndef DataToClientWriterChooser_hpp
#define DataToClientWriterChooser_hpp

#include <functional>
#include <map>

#include "../ClientConnectionType.hpp"
#include "../Client.hpp"

/**
*	@class DataToClientWriterChooser
*/
class DataToClientWriterChooser final
{
public:
	/**
	*	This function implements choosing of writer function according to client's connection to destination server.
	*	@param clientConnectionType Value representing the type client to destination server's connection.
	*	@see ClientConnectionType
	*	@returns Appropriate writer function according to the client's connection to destination server.
	*/
	static std::function<int(Client&)> chooseDataToClientWriter(ClientConnectionType clientConnectionType) {
		return DATA_TO_CLIENT_WRITERS_BY_CONNECTION_TYPE[clientConnectionType];
	}

private:

	static std::map<ClientConnectionType, std::function<int(Client&)>> DATA_TO_CLIENT_WRITERS_BY_CONNECTION_TYPE;
};

#endif // DataToClientWriterChooser_hpp