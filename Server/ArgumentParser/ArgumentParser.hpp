#pragma once

#include <boost/program_options.hpp>
#include <iostream>
#include <string>

class ArgumentParser
{
public:
	static void parse(int argc, char** argv)
	{
		namespace opts = boost::program_options;
		opts::options_description desc("Options");
		desc.add_options()
			("help,h", "See help page for this program")
			("encrypted,e", "Use encrypted connection between client and proxy")
			("normal,n", "Use unencrypted connection between client and proxy, this option is selected by default");

		opts::variables_map map;
		try
		{
			opts::store(opts::parse_command_line(argc, argv, desc), map);

			if(map.count("help"))
			{
				std::cout << "Proxy server" << std::endl
						  << desc << std::endl;
				exit(-1);
			}

			opts::notify(map);
		}
		catch(opts::error& e)
		{
			std::cerr << "An error occured parsing while command line: " << e.what() << std::endl << std::endl
					  << desc << std::endl;
			exit(-1);
		}
		
		// TODO:dodać sparsowane dane do settingsów

	}
private:
};