#ifndef _PSiCAlgorithm_hpp_
#define _PSiCAlgorithm_hpp_

#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <string>

using cpp_int = boost::multiprecision::cpp_int;

std::string PSiCAlgorithm(std::string input);

std::string& removeLeadingZeros(std::string& input);

#endif