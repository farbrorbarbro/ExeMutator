#pragma once

#include "InstructionNodes/InstructionList.h"


class ScrambleBlock
{
public:

	ScrambleBlock() {}
	
	void addInstruction(InstructionPtr &&instruction);
	InstructionList&	getInstructions();

private:

	InstructionList instructions;
};

