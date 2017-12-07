#pragma once

#include <vector>
#include <stdint.h>
#include "InstructionList.h"
#include "distorm/distorm.h"
#include "BranchType.h"


struct BranchRecord
{
	class BranchInstruction		*branchInstruction;
	uint32_t					jumpAddress;
};


class InstructionNodesBuilder
{
public:

	InstructionNodesBuilder(
		std::vector<uint8_t>	&inData,
		uint32_t				inVirtualAddress,
		InstructionList			&out_InstructionList
	);
	
	bool build();

private:

	bool createInstructionNodes();

	void addNormalInstruction(
		const _DInst	&inDecodedInstruction,
		uint8_t			*inData
	);

	void addControlFlowInstruction(
		const _DInst	&inDecodedInstruction, 
		uint8_t			*inData,
		BranchType		branchType
	);

	void linkLabels();

	class Instruction* getNodeWithVA(uint32_t virtualAddress);

private:

	std::vector<uint8_t>		&data;
	uint32_t					virtualAddress;
	InstructionList				&instructionList;

	std::vector<_DInst>			decodedInstructions;

	std::vector<BranchRecord>	branchRecords;
};

