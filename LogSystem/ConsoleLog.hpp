#ifndef ConsoleLog_hpp
#define ConsoleLog_hpp

#include <iostream>
#include <string>
#include <vector>

#include "Logging.hpp"

class ConsoleLog final : public Logging
{
public:

	void logMessage(const std::string& tags, const std::string& message){
		std::cout << tags << " " << message << std::endl;

	}

	static void logHttpRequest(const std::vector<char>& request) {
		std::cout << "[HTTP REQUEST]" << std::endl;
		for(auto& character : request) {
			std::cout << character;
		}
		std::cout << std::endl;
	}
};

#endif // ConsoleLog_hpp