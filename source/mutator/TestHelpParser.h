#pragma once

#include <string>


class TestHelpParser
{
public:

	TestHelpParser(const std::string& inInputString);
	
	std::string getNextWord();
	
	std::string parseOperator();

private:

	uint32_t index = 0;
	const std::string& inputString;
};

