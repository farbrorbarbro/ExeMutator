#pragma once

#include <stdint.h>


class InstructionLabel
{
public:

	InstructionLabel();
	InstructionLabel(class Instruction *inInstruction);

	uint32_t					getVirtualAddress() const;
	class Instruction*			getInstruction() const;

	int32_t						calculateRelativeJump(uint32_t currentAdress, uint32_t instructionSize) const;

	void						setInstruction(class Instruction *inInstruction);
	void						setJumpOffset(uint32_t inJumpOffset);

	bool						isValid() const;

private:

	class Instruction			*instruction	= nullptr;
	uint32_t					jumpOffset		= 0;
};
