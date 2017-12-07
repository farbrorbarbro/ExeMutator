#pragma once
#include "TextInstruction.h"
#include "InstructionNodes\InstructionList.h"


class TextInstructionsCompiler
{
public:

	TextInstructionsCompiler() {}
	
	void addInstruction(TextInstruction &&newInstruction);

	bool compile();

	uint32_t insertAt(
		InstructionList	&list, 
		uint32_t		index, 
		bool			bRemoveInstructionAtIndex
	);

	uint32_t getSize() const;

private:

	std::vector<TextInstruction>	textInstuctions;
	InstructionList					compiledInstructions;
};

