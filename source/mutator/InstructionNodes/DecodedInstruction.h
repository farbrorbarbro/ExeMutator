#pragma once

#include "Instruction.h"
#include "distorm/distorm.h"


class DecodedInstruction : public Instruction
{
public:

	DecodedInstruction(
		const _DInst	&inDecodedInstruction,
		uint8_t			*inData
	);
	
	const _DInst*	getDecodedInstruction() const override;

private:

	const _DInst decodedInstruction;
};

