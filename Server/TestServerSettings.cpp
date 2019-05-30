#include "ServerSettings.hpp"

#include <iostream>

int main() {
  ServerSettings* s = new ServerSettings();
  s->setCurrentTreeToDefault();
  s->printCurrentTree();
  s->readJson("http_connection_data_example.json");

  std::cout << "get test: " << s->getTimeoutLimit() << std::endl;

  s->setTimeoutLimit("10");
  s->printCurrentTree();
}
