#ifndef HttpRequestParser_hpp
#define HttpRequestParser_hpp

#include "HttpRequest.hpp"

#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <string>
#include <cstring>


class HttpRequestParser
{
public:

	static HttpRequest* parseHttpRequest(std::vector<char> httpRequest)
	{
		std::vector<char> requestLine;
		std::vector<std::vector<char>> httpRequestLines;
		for (unsigned int i=0; i<httpRequest.size(); ++i) {
			if (httpRequest[i] == '\n' || i == httpRequest.size()-1) {
				if (requestLine.back() == '\r'){
					requestLine.pop_back();
				}
				httpRequestLines.push_back(requestLine);
				requestLine.clear();
			} else {
				requestLine.push_back(httpRequest[i]);
			}
		}

		std::vector<char> requestFirstLine = httpRequestLines[0];
		std::vector<char> methodString;
		std::vector<char> pathString;
		std::vector<char> protocolString;

		for (unsigned i = 0; i < requestFirstLine.size(); ++i) {
			if (requestFirstLine[i] != ' ') {
				methodString.push_back(requestFirstLine[i]);
			} else {
				break;
			}
		}

		for (unsigned i = methodString.size() + 1; i < requestFirstLine.size(); ++i) {
			if (requestFirstLine[i] != ' ') {
				pathString.push_back(requestFirstLine[i]);
			} else {
				break;
			}
		}

		for (unsigned i = methodString.size() + pathString.size() + 2; i < requestFirstLine.size(); ++i) {
			protocolString.push_back(requestFirstLine[i]);
		}

		Method httpMethod = findHttpMethod(vectorToString(methodString));

		auto headers = processHeaders(httpRequestLines);

		std::vector<char> body;

		auto contentLengthHeader = std::find_if(headers.begin(), headers.end(), [](auto& header) {
			return header.first == "Content-Length";
		});

		if (contentLengthHeader != headers.end()) {
			std::istringstream iss (contentLengthHeader->second);
			unsigned contentLength = 0;
			iss >> contentLength;
			const char *crlf = "\r\n\r\n";
			std::vector<char>::iterator endOfHeadersPosition = std::search(httpRequest.begin(), httpRequest.end(), crlf, crlf + strlen(crlf));
			for (auto it = endOfHeadersPosition + 4; it != httpRequest.end(); ++it) {
				if (body.size() < contentLength) {
					body.push_back(*it);
				}
				else break;
				
			}
		}

		return new HttpRequest(httpMethod, pathString, protocolString, headers, body);
	}

	static std::vector<std::pair<std::string, std::string>> processHeaders(std::vector<std::vector<char>> httpRequestLines) {
		std::vector<std::pair<std::string, std::string>> headers;
		for (unsigned int i=1; i<httpRequestLines.size(); ++i) {

			std::vector<char> requestLine = httpRequestLines[i];

			auto delim = std::find(requestLine.begin(), requestLine.end(), ':');
			if (delim == requestLine.end()) {
				break;
			}
		
			std::vector<char> header(requestLine.begin(), delim);
			std::vector<char> value(delim + 2, requestLine.end());

			headers.push_back(std::make_pair(vectorToString(header), vectorToString(value)));

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
