#ifndef SSLDataWriter_hpp
#define SSLDataWriter_hpp

class SSLDataWriter final
{
public:

	static int write(Client& client) {
		std::string simpleReply = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\nContent-length: 4\r\n\r\ntest";

		int operationStatus = SSL_write(client.ssl, simpleReply.data(), simpleReply.size());

		return operationStatus;
	}

};

#endif // SSLDataWriter_hpp