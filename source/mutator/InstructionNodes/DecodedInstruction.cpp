#include "DecodedInstruction.h"


DecodedInstruction::DecodedInstruction(
	const _DInst	&inDecodedInstruction, 
	uint8_t			*inData
)
	: decodedInstruction { inDecodedInstruction }
{
	instructionLength = inDecodedInstruction.size;

	memcpy(instructionBytes, inData, inDecodedInstruction.size);
	setVirtualAddress((uint32_t)inDecodedInstruction.addr);
}

const _DInst* DecodedInstruction::getDecodedInstruction() const
{
	return &decodedInstruction;
}
