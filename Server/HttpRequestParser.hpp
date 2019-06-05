#ifndef HttpRequestParser_hpp
#define HttpRequestParser_hpp

#include "HttpRequest.hpp"

#include <sstream>
#include <iostream>
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
			if (httpRequest[i] == '\n' || i == httpRequest.size()-1) {
				httpRequestLines.push_back(requestLine);
				requestLine.clear();
			} else {
				requestLine.push_back(httpRequest[i]);
			}
		}

		if (httpRequestLines.size() == 0) {
			return nullptr;
		}

		else if (httpRequestLines.size() < 3) {
			// TODO: What if there are no headers?
		}

		std::cout << "Received Http Request " << std::endl;
		std::vector<char> httpMethodVector = httpRequestLines[0];
		Method httpMethod = findHttpMethod(vectorToString(httpMethodVector));
		std::cout << "Http method: " << vectorToString(httpMethodVector)  << std::endl;

		std::vector<char> pathToResource = httpRequestLines[1];
		const char* dir = "../Strona/";
		const char* length = dir + strlen(dir);
		pathToResource.insert(pathToResource.begin(), dir, length);
		std::cout << "Path to resource: " << vectorToString(pathToResource) << std::endl;

		std::map<std::string, std::string> headers = processHeaders(httpRequestLines);
		printHeaders(headers);

		return new HttpRequest(httpMethod, pathToResource, headers);
	}

	static std::map<std::string, std::string> processHeaders(std::vector<std::vector<char>> httpRequestLines) {
		std::map<std::string, std::string> headers;
		for (unsigned int i=2; i<httpRequestLines.size(); ++i) {
			std::vector<char> requestLine;
			std::vector<char> header;
			std::vector<char> value;
			bool stillHeader = true;

			requestLine = httpRequestLines[i];
			for (unsigned int j=0; j<requestLine.size(); ++j) {
				if (requestLine[j] == ':') {
					j += 2;
					stillHeader = false;
				}
				if (stillHeader) header.push_back(requestLine[j]);
				else value.push_back(requestLine[j]);
			}

			headers.insert(std::pair<std::string, std::string>(vectorToString(header), vectorToString(value)));

		}
		return headers;
	}

	static void printHeaders(std::map<std::string, std::string> headers) {
		std::cout << "Headers:" << std::endl;
		for(auto it = headers.begin(); it != headers.end(); ++it) {
    	std::cout << it->first << ": " << it->second << std::endl;
		}
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
