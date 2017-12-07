#include "Instruction.h"


uint32_t Instruction::getVirtualAddress() const
{
	return virtualAddress;
}

void Instruction::setVirtualAddress(uint32_t newVirtualAddress)
{
	virtualAddress = newVirtualAddress;
}

uint8_t Instruction::getInstructionSize() const
{
	return instructionLength;
}

const uint8_t* Instruction::getData() const
{
	return instructionBytes;
}
