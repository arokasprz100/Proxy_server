#include "PSiCAlgorithm.hpp"

std::string PSiCAlgorithm(std::string input)
{
	std::cout << input << std::endl;

	// step 1
	std::string last_24_digits_string(input.substr(2, input.size()));
	cpp_int last_24_digits(removeLeadingZeros(last_24_digits_string));
	std::cout << last_24_digits << std::endl;

	// step 2
	cpp_int multiplied = last_24_digits * 1000000;
	std::cout << multiplied << std::endl;

	// step 3
	cpp_int increased = multiplied + 252100;
	std::cout << increased << std::endl;

	// step 4
	cpp_int first_2_digits = increased;

	while(first_2_digits > 100)
		first_2_digits /= 10;

	std::cout << first_2_digits << std::endl;

	// step 5
	cpp_int result = last_24_digits + first_2_digits;
	std::cout << result << std::endl;

	// step 6
	cpp_int modulo = result % 97;
	std::cout << modulo << std::endl;

	return "";
}

std::string& removeLeadingZeros(std::string& input)
{
	input.erase(0, std::min(input.find_first_not_of('0'), input.size() - 1));
	return input;
}