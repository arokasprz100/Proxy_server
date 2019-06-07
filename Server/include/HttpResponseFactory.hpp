#ifndef HttpResponseFactory_hpp
#define HttpResponseFactory_hpp

#include <functional>
#include <vector>
#include <string>
#include <map>

class HttpResponseFactory final 
{
public:

	static std::vector<char> getResponseByCode(const std::string& code);

private:

	static std::vector<char> get200Response();

	static std::vector<char> get413Response();

	static std::vector<char> get501Response();

	static std::vector<char> get502Response();

	static std::map<std::string, std::function<std::vector<char>()>> RESPONSE_GETTERS_BY_CODE;
};

#endif // HttpResponseFactory_hpp