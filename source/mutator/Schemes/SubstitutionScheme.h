#pragma once
#include "MutationScheme.h"
#include "InstructionNodes\InstructionList.h"
#include <stdint.h>


class SubstitutionScheme : public MutationScheme
{
public:

	SubstitutionScheme() {}
	
	void applyScheme(
		std::vector<uint8_t>	&inData, 
		uint32_t				virtualAddress, 
		uint32_t				newVirtualAddress
	) override;

private:

	uint32_t substitutePushPop(InstructionList &instructionList
		, uint32_t index, const class TextInstruction &instruction, bool bIsPush);

	uint32_t substituteMov(InstructionList &instructionList, uint32_t index
		, const class TextInstruction &instruction);

	void generateRegisterSwap(
		class TextInstructionsCompiler	&helpCompiler, 
		const char						*reg1, 
		const char						*reg2
	);
};