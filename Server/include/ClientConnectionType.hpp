/**
*	@file ClientConnectionType.hpp
*	@brief This file contains the definition of an enum type describing the type of client to proxy connection.
*/

#ifndef ClientConnectionType_hpp
#define ClientConnectionType_hpp

/**
*	@enum This enum type describes the type of client to proxy connection.
*/
enum class ClientConnectionType {
	ENCRYPTED, /**< This value means the connection to proxy uses SSL. */
	UNENCRYPTED /**< This value means the connection to proxy uses plain HTTP. */
};

#endif // ClientConnectionType_hpp