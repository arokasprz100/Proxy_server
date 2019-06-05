cmake_minimum_required(VERSION 3.7.2)
project(ProxyServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -pedantic-errors")
set(CMAKE_SHARED_LINKER_FLAGS "-lssl -lcrypto")


file(GLOB Server_src
    "*.hpp"
    "*.cpp"
    "DataReaders/*.hpp"
    "DataReaders/*.cpp"
    "DataWriters/*.hpp"
    "DataWriters/*.cpp"
    "LogSystem/*.hpp"
    "LogSystem/*.cpp"
    "OperationStatusHandlers/*.hpp"
    "OperationStatusHandlers/*.cpp"
    "ServerSettings/*.hpp"
    "ServerSettings/*.cpp"
    "ClientAcceptors/*.hpp"
    "ClientAcceptors/*.cpp"
    "ArgumentParser/*.hpp")

add_executable(Server
	${Server_src}
)

find_package(OpenSSL REQUIRED)
FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
target_link_libraries(Server OpenSSL::SSL ${Boost_LIBRARIES})