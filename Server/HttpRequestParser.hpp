#ifndef HttpRequestParser_hpp
#define HttpRequestParser_hpp

#include "HttpRequest.hpp"

#include <sstream>
#include <vector>
#include <iostream>
#include <map>
#include <algorithm>

extern std::map<Method, std::string> httpMethodStringsMap;

class HttpRequestParser
{
public:

	static HttpRequest* parseHttpRequest(const std::string& httpRequest)
	{
		std::istringstream stream(httpRequest);
		std::string requestLine;
		std::vector<std::string> httpRequestLines;
		while (std::getline(stream, requestLine)) {
			httpRequestLines.push_back(requestLine);
		}

		if (httpRequestLines.size() == 0) {
			return nullptr;
		}

		std::string httpMethodString;
		std::string pathToResource;

		std::istringstream tokenStream(httpRequestLines.at(0));

		tokenStream >> httpMethodString;
		tokenStream >> pathToResource;
		pathToResource = "../Strona" + pathToResource;

		Method httpMethod = findHttpMethod(httpMethodString);

		std::cout << "Received Http Request " << std::endl;
		std::cout << "Http method : " << httpMethodString << std::endl;
		std::cout << "Path to resource: " << pathToResource << std::endl;

		return new HttpRequest(httpMethod, pathToResource);
	}

	static Method findHttpMethod(std::string httpMethodString) {
		auto result = std::find_if(httpMethodStringsMap.begin(),
															 httpMethodStringsMap.end(),
															 [httpMethodString](const auto& element) {
																 return element.second == httpMethodString;
															 });
	  if (result != httpMethodStringsMap.end()) return result->first;
		else { return Method::ERROR; }
	}

};

#endif // HttpRequestParser_hpp
