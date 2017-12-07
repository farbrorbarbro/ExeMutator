#pragma once

#include <string>


class TextInstruction
{
public:

	TextInstruction();
	
	TextInstruction(
		const char *inInstuctionName,
		const char *inOperator1 = nullptr,
		const char *inOperator2 = nullptr
	);

	TextInstruction(
		std::string &&inInstructionName,
		std::string &&inOperator1,
		std::string &&inOperator2
	);


	const std::string&	getInstructionName() const;
	void				setInstructionName(const char *newName);

	const std::string&	getOperator1() const;
	void				setOperator1(const char *newOperator);
	
	const std::string&	getOperator2() const;
	void				setOperator2(const char *newOperator);


	std::string getInstuctionAsString() const;


	static TextInstruction parseInstruction(const std::string &inInstuctionText);

private:

	std::string instructionName;
	std::string operator1;
	std::string operator2;
};

