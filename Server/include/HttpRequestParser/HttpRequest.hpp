/**
*	@file RequestURIParser.hpp
*	@brief 
*/

#ifndef HttpRequest_hpp
#define HttpRequest_hpp

#include <vector>
#include <map>
#include <string>
#include <sstream>

#include "RequestURIParser.hpp"
#include "../PSiCAlgorithm/PSiCAlgorithm.hpp"

/**
*	@enum Method This enum provides values for each HTTP request type.
*/
enum class Method {GET, POST, HEAD, PUT, DELETE, CONNECT, OPTIONS, TRACE, ERROR};


class HttpRequest
{
public:
	/**
	*	A constructor. Initializes HttpRequest object for later ease of use.
	*/
	HttpRequest(Method method, 
		const std::vector<char>& resourcePath, 
		const std::vector<char>& protocol, 
		const std::vector<std::pair<std::string, std::string>>& headers,
		const std::vector<char>& body
	) : m_method(method), 
		m_resourcePath(resourcePath), 
		m_protocol(protocol), 
		m_headers(headers),
		m_body(body) {}

	/**
	*	A getter function.
	*	@returns The type of HTTP method.
	*/
	Method getHttpMethod() const {
		return m_method;
	}

	/**
	*	A getter function.
	*	@returns HTTP request method as string.
	*/
	std::string getHttpMethodAsString() {
		return httpMethodStringsMap[m_method];
	}

	/**
	*	A getter function.
	*	@returns A string object containing the resource path from the HTTP request.
	*/
	std::string getResourcePath() const {
		return std::string(m_resourcePath.begin(), m_resourcePath.end());
	}

	/**
	*	A getter function.
	*	@returns A vector of header name and value pairs.
	*/
	std::vector<std::pair<std::string, std::string>> getHeaders() const {
		return m_headers;
	}
	
	/**
	*	A getter function.
	*	@returns The kind of connection found in HTTP request.
	*/
	std::string getProtocolFromResourcePath() const {
		const char* protocolDelim = "://";
		auto protocolNameEnd = std::search(m_resourcePath.begin(), m_resourcePath.end(), 
			protocolDelim, protocolDelim + strlen(protocolDelim));
		if (protocolNameEnd == m_resourcePath.end()) {
			return "";
		}
		return std::string(m_resourcePath.begin(), protocolNameEnd);
	}

	/**
	*	A getter function.
	*	@returns Size of the accumulated headers in the object.
	*/
	int getHeadersSize() const {
		int size = 0;
		for (auto& header : m_headers) {
			size += header.first.size() + header.second.size();
		}
		return size;
	}

	/**
	*	A getter function.
	*	@returns Number of headers found in the request.
	*/
	int getNumberOfHeaders() const {
		return m_headers.size();
	}

	/**
	*	A getter function.
	*	@returns The whole request as a vector of chars.
	*/
	std::vector<char> getFullRequest() {

		std::vector<char> request;
		std::string httpMethod = getHttpMethodAsString();
		request.insert(request.end(), httpMethod.begin(), httpMethod.end());
		request.push_back(' ');
		request.insert(request.end(), m_resourcePath.begin(), m_resourcePath.end());
		request.push_back(' ');
		request.insert(request.end(), m_protocol.begin(), m_protocol.end());
		request.push_back('\r');
		request.push_back('\n');

		if (m_headers.size() > 0) {

			for(auto& header : m_headers) {
				std::string headerName = header.first;
				std::string headerValue = header.second;
				request.insert(request.end(), headerName.begin(), headerName.end());
				request.push_back(':');
				request.push_back(' ');
				request.insert(request.end(), headerValue.begin(), headerValue.end());
				request.push_back('\r');
				request.push_back('\n');
			}
		}
		std::string headersEnd = "\r\n";
		request.insert(request.end(), headersEnd.begin(), headersEnd.end());
		request.insert(request.end(), m_body.begin(), m_body.end());
		return request;
	}

	/**
	*	This member function remves the local part from the request's resource path.
	*/
	void eraseNonLocalPartOfResourcePath() {
		RequestURI requestURI = RequestURIParser::parse(std::string(m_resourcePath.begin(), m_resourcePath.end()));
		const char* nonLocal = requestURI.nonLocalPart.c_str();

		auto afterNonLocal = std::search(m_resourcePath.begin(), 
								m_resourcePath.end(), 
								nonLocal, nonLocal + strlen(nonLocal)) + strlen(nonLocal);

		m_resourcePath.erase(m_resourcePath.begin(), afterNonLocal);

	}

	/**
	*	This member function checks whether the request has been received whole yet or not.
	*	@returns True if it's ready and false if not.
	*/
	bool isRequestComplete() {
		auto contentLengthHeader = std::find_if(m_headers.begin(), m_headers.end(), [](auto& header) {
			return header.first == "Content-Length";
		});
		if (contentLengthHeader != m_headers.end()) {
			std::istringstream iss (contentLengthHeader->second);
			unsigned contentLength = 0;
			iss >> contentLength;
			if (contentLength == m_body.size()) {
				return true;
			}
			return false;
		}
		return true;
	}

	/**
	*	This member function checks if there are number sequences specified in the task.
	*	@param numberSequence A reference to string object containing the number sequence the ones satisfying the conditions are to be swapped to.
	*/
	void changeNumberSequenceInBody(const std::string& numberSequence) {
		if (m_body.size() > 0) {
			PSiCAlgorithm::lookForSequences(m_body, numberSequence);
		}
	}

private:

	Method m_method;
	std::vector<char> m_resourcePath;
	std::vector<char> m_protocol;
	std::vector<std::pair<std::string, std::string>> m_headers;
	std::vector<char> m_body;

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

};

#endif // HttpRequest_hpp
