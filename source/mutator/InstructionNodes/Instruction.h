#pragma once

#include<stdint.h>
#include "distorm/distorm.h"
#include <vector>


class Instruction
{
public:

	virtual ~Instruction() {}

	// virtual address info
	uint32_t						getVirtualAddress() const;
	void							setVirtualAddress(uint32_t newVirtualAddress);

	// instruction data info
	uint8_t							getInstructionSize() const;
	const uint8_t*					getData() const;

	// branch instructions
	virtual bool					isControlFlow() const { return false; };
	virtual void					resolveLabelAddress() {};
	
	virtual bool					hasBranchToInstruction(Instruction *instruction) const { return false; }
	virtual void					setBranchInstruction(Instruction *instruction) {}
	virtual void					setBranchOffset(uint32_t jumpOffset) {}

	virtual const _DInst*			getDecodedInstruction() const { return nullptr; };

protected:

	uint32_t	virtualAddress			= 0;

	uint8_t		instructionLength		= 0;

	// max allowed instruction length is 15 bytes
	uint8_t		instructionBytes[15]	= { 0 }; 
};