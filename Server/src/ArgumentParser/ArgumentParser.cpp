#include "../../include/ArgumentParser/ArgumentParser.hpp"



InputSettings ArgumentParser::parse(int argc, char** argv)
{
	namespace opts = boost::program_options;
	opts::options_description desc("Options");
	desc.add_options()
		("help,h", "See help page for this program")
		("encrypted,e", "Use encrypted connection between client and proxy")
		("unencrypted,u", "Use unencrypted connection between client and proxy, this option is selected by default")
		("silent,s", "Do not display log messages, this option is selected by default")
		("verbose,v", "Display log messages to console")
		("menu,m", "Display interactive settings menu. Disabled by default.");

	opts::variables_map map;
	try
	{
		opts::store(opts::parse_command_line(argc, argv, desc), map);

		if(map.count("help"))
		{
			std::cout << "Proxy server" << std::endl
					  << desc << std::endl;
			exit(0);
		}

		opts::notify(map);
	}
	catch(opts::error& e)
	{
		std::cerr << "An error occured parsing while command line: " << e.what() << std::endl << std::endl
				  << desc << std::endl;
		exit(-1);
	}

	LogType logType = getLogSettings(map, desc);
	ClientConnectionType clientConnectionType = getClientConnectionSettings(map, desc);
	bool shouldRunSettingsMenu = getMenuSettings(map);
	
	return InputSettings(logType, clientConnectionType, shouldRunSettingsMenu);

}



LogType ArgumentParser::getLogSettings(const boost::program_options::variables_map& map, 
	const boost::program_options::options_description& desc) 
{
	if (map.count("silent") && map.count("verbose")) {
		std::cout << "[ERROR] Server can not be both verbose and silent" << std::endl
			<< desc << std::endl;
		exit(-1);
	}
	else if (map.count("verbose")) {
		return LogType::CONSOLE;
	}
	else {
		return LogType::SILENT;
	}
}



ClientConnectionType ArgumentParser::getClientConnectionSettings(const boost::program_options::variables_map& map, 
	const boost::program_options::options_description& desc) 
{
	if (map.count("encrypted") && map.count("unencrypted")) {
		std::cout << "[ERROR] Can not use both encrypted and unencrypted options" << std::endl
			<< desc << std::endl;
		exit(-1);
	}
	else if (map.count("encrypted")) {
		return ClientConnectionType::ENCRYPTED;
	}
	else {
		return ClientConnectionType::UNENCRYPTED;
	}
}



bool ArgumentParser::getMenuSettings(const boost::program_options::variables_map& map)
{
	return map.count("menu");
}