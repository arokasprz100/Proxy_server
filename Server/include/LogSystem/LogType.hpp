/**
*	@file LogType.hpp
*	@brief Contains definitions of enum type describing type of outputs possible in the porgram.
*/

#ifndef LogType_hpp
#define LogType_hpp

/**
*	@enum LogType Definition of enum type describing types of output.
*/
enum class LogType {
	CONSOLE, /**< Enum type describing output to console. */
	SILENT	/**< Enum type describing no output to console to be performed. */
};

#endif // LogType_hpp