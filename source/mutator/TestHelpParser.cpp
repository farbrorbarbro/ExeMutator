#include "TestHelpParser.h"
#include <ctype.h>


TestHelpParser::TestHelpParser(const std::string& inInputString)
	: inputString	{ inInputString }
{
}

std::string TestHelpParser::getNextWord()
{
	std::string result;

	while (index < inputString.size())
	{
		auto c = inputString[index];
		++index;

		if (isalpha(c) > 0)
		{
			result += c;
		}
		else
		{
			break;
		}
	}

	return result;
}

std::string TestHelpParser::parseOperator()
{
	std::string result;

	while (index < inputString.size())
	{
		auto c = inputString[index];
		
		if (c == ',')
		{
			index += 2;
			break;
		}

		if (c != '\n')
		{
			result += c;
		}

		++index;
	}

	return result;
}
