#ifndef HttpRequestParser_hpp
#define HttpRequestParser_hpp

#include "HttpRequest.hpp"

#include <sstream>
#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <string.h>


class HttpRequestParser
{
public:

	static HttpRequest* parseHttpRequest(std::vector<char> httpRequest)
	{
		std::vector<char> requestLine;
		std::vector<std::vector<char>> httpRequestLines;
		for (unsigned int i=0; i<httpRequest.size(); ++i) {
			if (httpRequest[i] != '\n') requestLine.push_back(httpRequest[i]);
			if (httpRequest[i] == '\n' || i == httpRequest.size()-1) {
				httpRequestLines.push_back(requestLine);
				requestLine.clear();
			}
		}

		if (httpRequestLines.size() == 0) {
			return nullptr;
		}

		std::vector<char> httpMethodVector = httpRequestLines[0];
		std::vector<char> pathToResource = httpRequestLines[1];
		const char* dir = "../Strona/";
		const char* length = dir + strlen(dir);
		pathToResource.insert(pathToResource.begin(), dir, length);
		std::vector<char> header = httpRequestLines[2];


		Method httpMethod = findHttpMethod(vectorToString(httpMethodVector));

		std::cout << "Received Http Request " << std::endl;
		std::cout << "Http method: " << vectorToString(httpMethodVector)  << std::endl;
		std::cout << "Path to resource: " << vectorToString(pathToResource) << std::endl;
		std::cout << "Header: " << vectorToString(header) << std::endl;

		return new HttpRequest(httpMethod, pathToResource, header);
	}


	static Method findHttpMethod(std::string httpMethodString) {
		std::map<Method, std::string> httpMethodStringsMap = {
			{ Method::GET, "GET" },
			{ Method::POST, "POST" },
			{ Method::HEAD, "HEAD" },
			{ Method::PUT, "PUT" },
			{ Method::DELETE, "DELETE" },
			{ Method::CONNECT, "CONNECT" },
			{ Method::OPTIONS, "OPTIONS" },
			{ Method::TRACE, "TRACE" },
		};

		auto result = std::find_if(httpMethodStringsMap.begin(),
															 httpMethodStringsMap.end(),
															 [httpMethodString](const auto& element) {
																 return element.second == httpMethodString;
															 });
	  if (result != httpMethodStringsMap.end()) return result->first;
		else { return Method::ERROR; }
	}

	static std::string vectorToString(std::vector<char> v) {
		std::string str(v.begin(), v.end());
		return str;
	}
};

#endif // HttpRequestParser_hpp
