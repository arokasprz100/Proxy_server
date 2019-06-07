/**
*	@file ArgumentParser.hpp
*	@brief Implements handling of command line arguments using [Boost].program_options.
*/

#pragma once

#include <boost/program_options.hpp>
#include <iostream>
#include <string>

#include "InputSettings.hpp"

/**
*	@class ArgumentParser
*/
class ArgumentParser
{
public:
	/**
	*	This function implements handling of command line arguments with [Boost].program_options
	*	@param argc Is the number of parameters passed to the program.
	*	@param argv Is pointer array of strings of values passed.
	*	@returns InputSettings object.
	*	@see InputSettings
	*/
	static InputSettings parse(int argc, char** argv);

private:

	static LogType getLogSettings(const boost::program_options::variables_map& map, 
		const boost::program_options::options_description& desc);

	static ClientConnectionType getClientConnectionSettings(const boost::program_options::variables_map& map, 
		const boost::program_options::options_description& desc);

	static bool getMenuSettings(const boost::program_options::variables_map& map);

};