#include "HttpRequestParser.hpp"

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
