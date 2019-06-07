#ifndef HttpRequest_hpp
#define HttpRequest_hpp

#include <vector>
#include <map>
#include <string>
#include <sstream>

#include "RequestURIParser.hpp"

enum class Method {GET, POST, HEAD, PUT, DELETE, CONNECT, OPTIONS, TRACE, ERROR};


class HttpRequest
{
public:

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

	Method getHttpMethod() const {
		return m_method;
	}

	std::string getHttpMethodAsString() {
		return httpMethodStringsMap[m_method];
	}

	std::string getResourcePath() const {
		return std::string(m_resourcePath.begin(), m_resourcePath.end());
	}

	std::vector<std::pair<std::string, std::string>> getHeaders() const {
		return m_headers;
	}
	
	std::string getProtocolFromResourcePath() const {
		const char* protocolDelim = "://";
		auto protocolNameEnd = std::search(m_resourcePath.begin(), m_resourcePath.end(), 
			protocolDelim, protocolDelim + strlen(protocolDelim));
		if (protocolNameEnd == m_resourcePath.end()) {
			return "";
		}
		return std::string(m_resourcePath.begin(), protocolNameEnd);
	}

	int getHeadersSize() const {
		int size = 0;
		for (auto& header : m_headers) {
			size += header.first.size() + header.second.size();
		}
		return size;
	}

	int getNumberOfHeaders() const {
		return m_headers.size();
	}

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

	void eraseNonLocalPartOfResourcePath() {
		RequestURI requestURI = RequestURIParser::parse(std::string(m_resourcePath.begin(), m_resourcePath.end()));
		const char* nonLocal = requestURI.nonLocalPart.c_str();

		auto afterNonLocal = std::search(m_resourcePath.begin(), 
								m_resourcePath.end(), 
								nonLocal, nonLocal + strlen(nonLocal)) + strlen(nonLocal);

		m_resourcePath.erase(m_resourcePath.begin(), afterNonLocal);

	}

	bool isRequestComplete() {
		auto contentLengthHeader = std::find_if(m_headers.begin(), m_headers.end(), [](auto& header) {
			return header.first == "Content-Length";
		});
		if (contentLengthHeader != m_headers.end()) {
			std::istringstream iss (contentLengthHeader->first);
			unsigned contentLength = 0;
			iss >> contentLength;
			if (contentLength == m_body.size()) {
				return true;
			}
			return false;
		}
		return true;
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
