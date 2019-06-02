#ifndef HttpRequest_hpp
#define HttpRequest_hpp

#include <vector>

enum class Method {GET, POST, HEAD, PUT, DELETE, CONNECT, OPTIONS, TRACE, ERROR};

#include <string>


class HttpRequest
{
public:

	HttpRequest(Method method, const std::vector<char>& resourcePath, const std::vector<char>& header)
		: m_method(method), m_resourcePath(resourcePath), m_header(header) {}

	Method getHttpMethod() const {
		return m_method;
	}

	const std::vector<char> getResourcePath() const {
		return m_resourcePath;
	}

	const std::vector<char> getHeader() const {
		return m_header;
	}

private:

	Method m_method;
	std::vector<char> m_resourcePath;
	std::vector<char> m_header;

};

#endif // HttpRequest_hpp
