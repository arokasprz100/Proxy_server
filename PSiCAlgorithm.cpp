#include "PSiCAlgorithm.hpp"

std::string PSiCAlgorithm(std::string input)
{
	std::cout << "PSiCAlgorithm input: " << input << std::endl;

	// step 1
	std::string last_24_digits_string(input.substr(2, input.size()));
	cpp_int last_24_digits(removeLeadingZeros(last_24_digits_string));

	// step 2
	cpp_int multiplied = last_24_digits * 1000000;

	// step 3
	cpp_int increased = multiplied + 252100;

	// step 4
	cpp_int first_2_digits = increased / cpp_int("10000000000000000000000000000");

	// step 5
	cpp_int result = last_24_digits + first_2_digits;

	// step 6
	cpp_int modulo = result % 97;
	std::cout << "modulo result: " << modulo << std::endl;

	if(modulo == 1)
		return "PLACEHOLDER STRING TO BE CHANGED WHEN SETTINGS ARE IMPLEMENTED";
	else
		return input;
}

std::string& removeLeadingZeros(std::string& input)
{
	input.erase(0, std::min(input.find_first_not_of('0'), input.size() - 1));
	return input;
}