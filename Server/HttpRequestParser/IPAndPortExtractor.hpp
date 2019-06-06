#ifndef IPAndPortExtractor_hpp
#define IPAndPortExtractor_hpp

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <string>
#include <tuple>
#include <iostream>

class IPAndPortExtractor
{
public:

	static std::tuple<std::string, int> extractIPAddressAndPortNumberFromURI(const std::string& hostname) {

		int portNumber = 80;
		std::string ipAddress = "";

		std::string toResolve = hostname;

		if (containsPortNumber(hostname)) {
			auto [hostName, port] = getHostNameAndPort(hostname);
			toResolve = hostName;
			portNumber = port;
		}

		if (isIPAlready(toResolve)) {
			return std::make_tuple(toResolve, portNumber);
		}

		addrinfo hints, *res;
		char addrstr[100] = {'\0'};
		void *ptr;

		memset (&hints, 0, sizeof (hints));
  		hints.ai_family = PF_UNSPEC;
  		hints.ai_socktype = SOCK_STREAM;
  		hints.ai_flags |= AI_CANONNAME;

  		std::string result = "[COULD NOT EXTRACT ADDRESS]";

  		getaddrinfo (toResolve.c_str(), NULL, &hints, &res);
	    while (res)
		{
			inet_ntop (res->ai_family, res->ai_addr->sa_data, addrstr, 99);

			switch (res->ai_family)
			{
				case AF_INET:
					ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
					break;
				case AF_INET6:
					ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
					break;
			}

			inet_ntop (res->ai_family, ptr, addrstr, 99);

			if (res->ai_family == PF_INET) {
				result = addrstr;
			}

			res = res->ai_next;
		}
		return std::make_tuple(result, portNumber);
	}

private:

	static std::tuple<std::string, int> getHostNameAndPort(const std::string& nonLocalURIPart) {
		size_t colonPosition = nonLocalURIPart.find_last_of(':');
		std::string hostName = nonLocalURIPart.substr(0, colonPosition);
		int port = std::stoi(nonLocalURIPart.substr(colonPosition + 1, nonLocalURIPart.size() - colonPosition - 1));
		return std::make_tuple(hostName, port);
	}


	static bool containsPortNumber(const std::string& nonLocalURIPart) {
		return nonLocalURIPart.find(':') != std::string::npos;
	}

	static bool isIPAlready(const std::string& nonLocalURIPart) {
		char buffer[100] = {'\0'};
		int result = inet_pton(AF_INET, nonLocalURIPart.data(), buffer);
		return result != 0;
	}

};

#endif // IPAndPortExtractor_hpp