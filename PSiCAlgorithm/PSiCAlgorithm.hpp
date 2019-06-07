#ifndef _PSiCAlgorithm_hpp_
#define _PSiCAlgorithm_hpp_

#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <string>
#include <vector>

using cpp_int = boost::multiprecision::cpp_int;

class PSiCAlgorithm
{
public:
	static void lookForSequences(std::vector<char>& input);

	// TODO: change to string in the settings
	static const std::string desiredSequenceString;

private:
	static bool checkSequence(std::string input);

	static std::string& removeLeadingZeros(std::string& input);
};

#endif