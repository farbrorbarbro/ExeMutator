#include "InstructionLabel.h"
#include "Instruction.h"


InstructionLabel::InstructionLabel()
{
}

InstructionLabel::InstructionLabel(class Instruction *inInstruction)
	: instruction	{ inInstruction }
{
}

uint32_t InstructionLabel::getVirtualAddress() const
{
	return instruction->getVirtualAddress();
}

class Instruction* InstructionLabel::getInstruction() const
{
	return instruction;
}

int32_t InstructionLabel::calculateRelativeJump(uint32_t currentAdress, uint32_t instructionSize) const
{
	if (instruction)
	{
		return instruction->getVirtualAddress() - (currentAdress + instructionSize);
	}
	else
	{
		return jumpOffset - (currentAdress + instructionSize);
	}
}

void InstructionLabel::setInstruction(class Instruction *inInstruction)
{
	instruction	= inInstruction;
	jumpOffset	= 0;
}

void InstructionLabel::setJumpOffset(uint32_t inJumpOffset)
{
	jumpOffset	= inJumpOffset;
	instruction	= nullptr;
}

bool InstructionLabel::isValid() const
{
	return instruction != nullptr;
}
