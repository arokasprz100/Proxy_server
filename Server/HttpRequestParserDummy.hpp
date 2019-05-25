#pragma once

#include <string>
#include <iostream>

struct HttpRequest{
	std::string method;
	std::string hostname;
};

class HttpRequestParserDummy
{
public:

	static HttpRequest parse(const std::string& request) {
		size_t afterMethod = request.find(" ");
		std::string method = request.substr(0, afterMethod);
		size_t afterHostname = request.find(" HTTP");
		std::string hostname = request.substr(method.size() + 1, afterHostname - afterMethod - 1);

		HttpRequest result;
		result.method = method;
		result.hostname = hostname;
		return result;
	}
};