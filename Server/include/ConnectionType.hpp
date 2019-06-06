/**
*	@file ConnectionType.hpp
*	@brief This file contains the definition of an enum type describing the type of client to destination server connection.
*/

#ifndef ConnectionType_hpp
#define ConnectionType_hpp

/**
*	@enum This enum type describes the type of client to destination server connection.
*/
enum class ConnectionType {
	ENCRYPTED, /**< This value means the connection to destination server uses SSL. */
	PLAIN_TEXT, /**< This value means the connection to destination server uses plain text HTTP. */
	UNDEFINED /**< This value means the connection to destination server has not yet been established. */
};

#endif // ConnectionType_hpp