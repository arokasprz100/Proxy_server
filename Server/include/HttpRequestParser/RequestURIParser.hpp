/**
*	@file RequestURIParser.hpp
*	@brief This file contains definitions of RequestURI struct and RequestURIParser class.
*/

#ifndef RequestURIParser_hpp
#define RequestURIParser_hpp

/**
*	@struct RequestURI
*/
struct RequestURI {
	std::string localPart;
	std::string nonLocalPart;
};

/**
*	@class RequestURIParser
*/
class RequestURIParser
{
public:
	/**
	*	This member fuction parses given URI and returns the local and non-local part.
	*	@param requestURI URI extracted from HTTP request.
	*	@returns RequestURI struct containing the local and non-local part of the given URI.
	*/
	static RequestURI parse(const std::string& requestURI) {
		std::string nonLocalURIPart = extractNonLocalURIPart(requestURI);
		std::string localURIPart = extractLocalURIPart(requestURI, nonLocalURIPart);

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