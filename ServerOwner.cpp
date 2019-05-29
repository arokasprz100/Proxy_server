#include "ServerOwner.hpp"

Server* ServerOwner::server = nullptr;

void sigintHandler(int sig_num) 
{ 
	std::cout << "Signal : " << sig_num << std::endl;
    std::cout << "Performing clean-up before exiting." << std::endl; 
    fflush(stdout);
    ServerOwner::stopServer();
    exit(1);
} 