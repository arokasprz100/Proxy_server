cmake_minimum_required(VERSION 3.7.2)
project(ProxyServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -pedantic-errors")
set(CMAKE_SHARED_LINKER_FLAGS "-lssl -lcrypto")

option(BUILD_DOC "Build documentation" ON)

find_package(Doxygen)
if (DOXYGEN_FOUND)
	SET(DOXYGEN_OUTPUT_DIRECTORY ./docs)
	SET(DOXYGEN_GENERATE_LATEX YES)
	SET(DOXYGEN_PROJECT_NAME "Proxy Server")
	SET(DOXYGEN_PROJECT_BRIEF "This program is a proxy server allowing both encrypted and unencrypted connection to the proxy from the client.")
    doxygen_add_docs(doc_doxygen
    				./include
    				VERBATIM)
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

file(GLOB Server_src
    "include/*/*.hpp"
    "include/*.hpp"
    "src/*/*.cpp"
    "src/*.cpp")

add_executable(Server
	${Server_src}
)

find_package(OpenSSL REQUIRED)
FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
target_link_libraries(Server OpenSSL::SSL ${Boost_LIBRARIES})