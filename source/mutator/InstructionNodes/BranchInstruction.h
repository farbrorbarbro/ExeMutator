#pragma once

#include "Instruction.h"
#include "InstructionLabel.h"
#include "BranchType.h"
#include "distorm/distorm.h"


class BranchInstruction : public Instruction
{
public:

	BranchInstruction(BranchType inBranchType);
	
	bool			isControlFlow() const override;
	void			resolveLabelAddress() override;

	bool			hasBranchToInstruction(Instruction *instruction) const override;

	void			setBranchInstruction(Instruction *instruction) override;
	void			setBranchOffset(uint32_t jumpOffset) override;

private:

	void initiateInstruction();

private:

	BranchType			branchType;
	InstructionLabel	label;
};

