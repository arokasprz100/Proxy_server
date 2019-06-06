/**
*	@file LogSystem.hpp
*	@brief 
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
*	@typedef AllArgsAreStrings
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
	*	
	*	@param logType
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
	*	
	*	@param message
	*	@param tags
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