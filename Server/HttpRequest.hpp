#ifndef HttpRequest_hpp
#define HttpRequest_hpp

enum class Method {GET, POST, HEAD, PUT, DELETE, CONNECT, OPTIONS, TRACE, ERROR};

#include <string>


class HttpRequest
{
public:

	HttpRequest(Method method, const std::string& resourcePath)
		: m_method(method), m_resourcePath(resourcePath) {}

	Method getHttpMethod() const {
		return m_method;
	}

	const std::string getResourcePath() const {
		return m_resourcePath;
	}

private:

	Method m_method;
	std::string m_resourcePath;

};

#endif // HttpRequest_hpp
