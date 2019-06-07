/**
*	@file LogSystem.hpp
*	@brief This file contains the definition of the implementation of log messages in the program.
*/

#ifndef LogSystem_hpp
#define LogSystem_hpp

#include "Logging.hpp"
#include "ConsoleLog.hpp"
#include "SilentLog.hpp"
#include "LogType.hpp"

#include <map>
#include <memory>
#include <type_traits>
#include <string>

/**
*	@typedef AllArgsAreStrings An alias for checking whether all parameters in a parameter pack are of type std::string.
*/
template<typename... Types>
using AllArgsAreStrings = typename std::conjunction<std::is_convertible<Types, std::string>...>::type;

/**
*	@class LogSystem
*/
class LogSystem
{
public:
	/**
	*	This function sets whether the logging functions will produce output to std::out or not.
	*	@param logType Desrcribes the chosen type of output.
	*	@see LogType
	*/
	static void initialize(LogType logType) {
		switch(logType) {
			case LogType::CONSOLE:
				logger = std::unique_ptr<Logging>(new ConsoleLog());
				break;
			case LogType::SILENT:
				logger = std::unique_ptr<Logging>(new SilentLog());
				break;
		}

		isInitialized = true;

	}

	/**
	*	This function takes variable amount of arguments and calls appropriate logging function depending of the type that was picked in program's options.
	*	@see ArgumentParser	
	*	@param message A reference to a string object containing the message attached to the tags.
	*	@param tags	A variable amount of string object containing the values to be highlighted. The brackets are added in this function.
	*/
	template<typename... Types>
	static void logMessage(const std::string& message, Types ... tags) {
		if (!isInitialized) {
			return;
		}
		static_assert(AllArgsAreStrings<Types...>::value, 
			"All arguments of LogSystem::logMessage function must be of type std::string.");

		logger->logMessage((addBrackets(tags) + ...), message);
	}

private:

	static std::string addBrackets(const std::string& tag) {
		return "[" + tag + "]";
	}


	static std::unique_ptr<Logging> logger;
	static bool isInitialized;

};

#endif // LogSystem_hpp