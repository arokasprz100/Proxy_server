#ifndef HttpRequest_hpp
#define HttpRequest_hpp

#include <vector>

enum class Method {GET, POST, HEAD, PUT, DELETE, CONNECT, OPTIONS, TRACE, ERROR};

#include <string>
#include <map>

class HttpRequest
{
public:

	HttpRequest(Method method, const std::vector<char>& resourcePath, const std::map<std::string, std::string> headers)
		: m_method(method), m_resourcePath(resourcePath), m_headers(headers) {}

	Method getHttpMethod() const {
		return m_method;
	}

	const std::vector<char> getResourcePath() const {
		return m_resourcePath;
	}

	const std::map<std::string, std::string> getHeader() const {
		return m_headers;
	}

private:

	Method m_method;
	std::vector<char> m_resourcePath;
	std::map<std::string, std::string> m_headers;

};

#endif // HttpRequest_hpp
