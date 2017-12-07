#include "TextInstruction.h"
#include "TestHelpParser.h"


TextInstruction::TextInstruction()
{
}

TextInstruction::TextInstruction(
	const char *inInstuctionName, 
	const char *inOperator1, 
	const char *inOperator2
)
{
	setInstructionName(inInstuctionName);
	setOperator1(inOperator1);
	setOperator2(inOperator2);
}

TextInstruction::TextInstruction(
	std::string &&inInstructionName,
	std::string &&inOperator1,
	std::string &&inOperator2
)
	: instructionName	{ std::move(inInstructionName) }
	, operator1			{ std::move(inOperator1) }
	, operator2			{ std::move(inOperator2) }
{
}

const std::string& TextInstruction::getInstructionName() const
{
	return instructionName;
}

void TextInstruction::setInstructionName(const char *newName)
{
	instructionName.clear();
	
	if (newName)
	{
		instructionName = newName;
	}
}

const std::string& TextInstruction::getOperator1() const
{
	return operator1;
}

void TextInstruction::setOperator1(const char *newOperator)
{
	operator1.clear();

	if (newOperator)
	{
		operator1 = newOperator;
	}
}

const std::string& TextInstruction::getOperator2() const
{
	return operator2;
}

void TextInstruction::setOperator2(const char *newOperator)
{
	operator2.clear();

	if (newOperator)
	{
		operator2 = newOperator;
	}
}

std::string TextInstruction::getInstuctionAsString() const
{
	std::string instructionText;
	
	instructionText += instructionName;
	
	if(operator1.size() > 0)
	{
		instructionText += " ";
		instructionText += operator1;

		if (operator2.size() > 0)
		{
			instructionText += ", ";
			instructionText += operator2;
		}
	}

	return instructionText;
}

TextInstruction TextInstruction::parseInstruction(const std::string &inInstuctionText)
{
	TestHelpParser helpParser(inInstuctionText);
	auto instructionName	= helpParser.getNextWord();
	auto operator1			= helpParser.parseOperator();
	auto operator2			= helpParser.parseOperator();

	TextInstruction newInstruction(
		std::move(instructionName),
		std::move(operator1),
		std::move(operator2)
	);

	return newInstruction;
}
