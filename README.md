# Proxy_server
This program is a proxy server that allows both encrypted and unencrypted connection to the proxy from the client.

## Compilation
To be able to use this software you need to compile it first. To do so, you need to navigate to the [Server](Server) directory. Then you need to run:
```bash
	cmake CMakeList.txt
```
This command will generate a proper Makefile and some other files. Next, type:
```bash
	make
```
To do this you need cmake version at least 3.9.6 as well as make installed on top of all the other libraries mentioned in the About section.

## Usage
It's worth noting that the generated Makefile will have an additional target, namely, running:

will produce this documentation! Be aware that this doesnt check if the current one is up-to-date and will produce it from scratch. Produced documentation is provided in HTML and LaTeX.

You can use our program with multiple command line options:
* -h [\-\-help] To see the help page for this program
* -e [\-\-encrypted] Use encrypted connection between client and proxy
* -u [\-\-unencrypted] Use unencrypted connection between client and proxy, this option is selected by default
* -s [\-\-silent ] Do not display log messages, this option is selected by defualt
* -v [\-\-verbose ] Display log messages to console
* -m [\-\-menu ] Display interactive settings menu. Disabled by default.


## About
Compilation of this program requires you to have some libraries installed:
* [Boost].multiprecision
* [Boost].program_options
* [Boost].proprety_tree
* OpenSSL

On top of that a compiler supporting C++17's fold expressions and parameter pack is needed, such as g++ version 8.2. If that is not your default compiler you can specify a flag to cmake to set it manually.

```bash
cmake -DCMAKE_CXX_COMPILER=g++-8.2 CMakeLists.txt 
```

## Authors
* [Michalina Oleksy](https://github.com/Robin-Bjornsdotter)
* [Arkadiusz Kasprzak](https://github.com/arokasprz100)
* [Dominik Dziuba](https://github.com/dz1domin)
