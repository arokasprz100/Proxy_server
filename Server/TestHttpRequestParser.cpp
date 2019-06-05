#include "HttpRequestParser.hpp"

int main() {
  std::vector<char> httpRequest = { 'a', 'a', 'a', '\n',
                                    'b', 'b', 'b', '\n',
                                    'c', 'c', 'c', ':', ' ', 'd', 'd', 'd', '\n',
                                    'e', 'e', 'e', ':', ' ', 'f', 'f', 'f', '\n' };
  HttpRequestParser::parseHttpRequest(httpRequest);
}
