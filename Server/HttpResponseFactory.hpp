#ifndef HttpResponseFactory_hpp
#define HttpResponseFactory_hpp

class HttpResponseFactory final 
{
public:

	static std::vector<char> get200Response() {
		std::string response = std::string("HTTP/1.1 200 Connection Established\r\n") + 
			"Reason: Connection Established\r\n" + 
			"Proxy-Connection: Keep-Alive\r\n" + 
			"Connection: Keep-Alive\r\n\r\n";
		return std::vector<char>(response.begin(), response.end());
	}

	static std::vector<char> get413Response() {
		std::string response = ("HTTP/1.1 413 Payload Too Large\r\n\r\n");
		return std::vector<char>(response.begin(), response.end());
	}
};

#endif // HttpResponseFactory_hpp