#include "PSiCAlgorithm.hpp"

const std::string PSiCAlgorithm::desiredSequenceString = "12345678901234567890123456";

bool PSiCAlgorithm::checkSequence(std::string input)
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
	cpp_int first_2_digits = cpp_int(input.substr(0, 2));

	// step 5
	cpp_int result = increased + first_2_digits;

	// step 6
	cpp_int modulo = result % 97;
	std::cout << "modulo result: " << modulo << std::endl;

	if(modulo == 1)
		return true;
	else
		return false;
}

std::string& PSiCAlgorithm::removeLeadingZeros(std::string& input)
{
	input.erase(0, std::min(input.find_first_not_of('0'), input.size() - 1));
	return input;
}

void PSiCAlgorithm::lookForSequences(std::vector<char>& input)
{
	std::string found = "";
	int firstIndex = 0;
	for(unsigned int i = 0; i < input.size(); i++)
	{
		char& curr = input[i];
		
		if(curr >= '0' && curr <= '9')
		{
			// beginning of the sequence to swap
			if(found.size() == 0)
				firstIndex = i;
			found += curr;

			if(found.size() == 26 && PSiCAlgorithm::checkSequence(found))
			{
				// swap chars to desired sequence
				// TODO: change to the variable in the settings
				for(int j = 0, seqIndex = firstIndex; j < 26;)
				{
					char& seqChar = input[seqIndex++];

					if(seqChar >= '0' && seqChar <= '9')
						seqChar = PSiCAlgorithm::desiredSequenceString.at(j++);
				}

				found.clear();
			}
		}
		else if(curr != ' ')
		{
			// clearing
			firstIndex = 0;
			found.clear();
		}
	}
}