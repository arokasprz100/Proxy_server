/**
*	@file HttpResponseFactory.hpp
*	@brief This file contains the definition of a class that implements discerning of received HTTP requests and returning approriate set of headers given the request's code.
*/

#ifndef HttpResponseFactory_hpp
#define HttpResponseFactory_hpp

#include <functional>
#include <vector>
#include <string>
#include <map>
/**
*	@class HttpResponseFactory
*/
class HttpResponseFactory final 
{
public:
	/**
	*	This function returns the appropriate response for a given HTTP code.
	*	@param code A reference to a string containing the 3 character code.
	*	@returns HTTP response appropriate to the code.
	*/
	static std::vector<char> getResponseByCode(const std::string& code);

private:

	static std::vector<char> get200Response();

	static std::vector<char> get413Response();

	static std::vector<char> get501Response();

	static std::vector<char> get502Response();

	static std::map<std::string, std::function<std::vector<char>()>> RESPONSE_GETTERS_BY_CODE;
};

#endif // HttpResponseFactory_hpp