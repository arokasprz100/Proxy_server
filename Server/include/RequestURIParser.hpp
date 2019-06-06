#ifndef RequestURIParser_hpp
#define RequestURIParser_hpp

struct RequestURI {
	std::string localPart;
	std::string nonLocalPart;
};

class RequestURIParser
{
public:

	static RequestURI parse(const std::string& requestURI) {
		std::string nonLocalURIPart = extractNonLocalURIPart(requestURI);
		// std::cout << "[NON_LOCAL] = " << nonLocalURIPart << std::endl;
		std::string localURIPart = extractLocalURIPart(requestURI, nonLocalURIPart);
		// std::cout << "[LOCAL] = " << localURIPart << std::endl;

		RequestURI result;
		result.localPart = localURIPart;
		result.nonLocalPart = nonLocalURIPart;

		return result;
	}

private:

	static std::string extractNonLocalURIPart(const std::string requestURI) {
		size_t nonLocalPartBegin = requestURI.find("//") + 2;
		size_t nonLocalPartEnd = nonLocalPartBegin;
		while (true) {
			if (requestURI[nonLocalPartEnd] == '/') break; else ++nonLocalPartEnd;
		}
		return requestURI.substr(nonLocalPartBegin, nonLocalPartEnd - nonLocalPartBegin);

	}

	static std::string extractLocalURIPart(const std::string requestURI, const std::string nonLocalURIPart) {
		size_t nonLocalPartEnd = requestURI.find(nonLocalURIPart) + nonLocalURIPart.size();
		return requestURI.substr(nonLocalPartEnd, requestURI.size() - nonLocalPartEnd);
	}

};

#endif // RequestURIParser_hpp