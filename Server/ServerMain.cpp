#include "Server.hpp"
#include <memory>

int main()
{
	std::unique_ptr<Server> server(new Server(8080));
	server->startHandlingClients();
	return 0;
}