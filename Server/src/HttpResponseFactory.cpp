#include "../include/HttpResponseFactory.hpp"

std::map<std::string, std::function<std::vector<char>()>> 
	HttpResponseFactory::RESPONSE_GETTERS_BY_CODE = 
{
	{
		"200", [](){ return get200Response(); }
	},
	{
		"413", [](){ return get413Response(); }
	},
	{
		"501", [](){ return get501Response(); }
	},
	{
		"502", [](){ return get502Response(); }
	}
};

std::vector<char> HttpResponseFactory::getResponseByCode(const std::string& code) {
	return RESPONSE_GETTERS_BY_CODE[code]();
}


std::vector<char> HttpResponseFactory::get200Response() {
	std::string response = std::string("HTTP/1.1 200 Connection Established\r\n") + 
		"Reason: Connection Established\r\n" + 
		"Proxy-Connection: Keep-Alive\r\n" + 
		"Connection: Keep-Alive\r\n\r\n";
	return std::vector<char>(response.begin(), response.end());
}

std::vector<char> HttpResponseFactory::get413Response() {
	std::string response = "HTTP/1.1 413 Payload Too Large\r\n\r\n";
	return std::vector<char>(response.begin(), response.end());
}

std::vector<char> HttpResponseFactory::get501Response() {
	std::string response = "HTTP/1.1 501 Not Implemented\r\n\r\n";
	return std::vector<char>(response.begin(), response.end());
}

std::vector<char> HttpResponseFactory::get502Response() {
	std::string response = "HTTP/1.1 502 Bad Gateway\r\n\r\n";
	return std::vector<char>(response.begin(), response.end());
}