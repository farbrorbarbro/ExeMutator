#include "ScrambleBlock.h"


void ScrambleBlock::addInstruction(InstructionPtr &&instruction)
{
	getInstructions().push_back(std::move(instruction));
}

InstructionList& ScrambleBlock::getInstructions()
{
	return instructions;
}
