#include "HttpRequestParser.hpp"

int main() {
  std::vector<char> httpRequest = {'a', 'a', 'a', '\n', 'b', 'b', 'b', '\n', 'c', 'c', 'c'};
  HttpRequestParser::parseHttpRequest(httpRequest);
}
