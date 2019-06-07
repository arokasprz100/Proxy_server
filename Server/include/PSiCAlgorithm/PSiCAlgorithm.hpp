/**
*	@file PSiCAlgorithm.hpp
*	@brief This file contains the definition of PSiCAlgorithm that implements handling sequences specified in the task.
*/

#ifndef _PSiCAlgorithm_hpp_
#define _PSiCAlgorithm_hpp_

#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <string>
#include <vector>

using cpp_int = boost::multiprecision::cpp_int;
/**
*	@class PSiCAlgorithm
*/
class PSiCAlgorithm
{
public:
	/**
	*	This member function implements looking for and replacing found sequences(in HTTP request body) if they fit the criteria.
	*	@param input A reference to std::vector of chars to the HTTP request's body being parsed by the proxy.
	*	@param desiredSequenceString A reference to string object containing the 26 number sequence the found one is to be swapped with.
	*/
	static void lookForSequences(std::vector<char>& input, const std::string& desiredSequenceString);

private:
	static bool checkSequence(std::string input);

	static std::string& removeLeadingZeros(std::string& input);
};

#endif