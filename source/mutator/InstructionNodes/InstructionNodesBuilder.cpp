#include "InstructionNodesBuilder.h"
#include "distorm\mnemonics.h"
#include "Instruction.h"
#include "CodeAnalysHelper.h"
#include "DecodedInstruction.h"
#include "BranchInstruction.h"


InstructionNodesBuilder::InstructionNodesBuilder(
	std::vector<uint8_t>	&inData,
	uint32_t				inVirtualAddress,
	InstructionList			&out_InstructionList
)
	: data				{ inData }
	, virtualAddress	{ inVirtualAddress }
	, instructionList	{ out_InstructionList }
{
}

bool InstructionNodesBuilder::build()
{
	// decode instructions
	if (!CodeAnalysHelper::decodeInstructions(data, virtualAddress, decodedInstructions))
	{
		return false;
	}

	// create instruction nodes
	if (!createInstructionNodes())
	{
		return false;
	}

	linkLabels();

	return true;
}

bool InstructionNodesBuilder::createInstructionNodes()
{
	for (auto &decodedInstruction : decodedInstructions)
	{
		auto opcode = decodedInstruction.opcode;

		uint8_t *rawDataPtr = data.data() + (decodedInstruction.addr - virtualAddress);

		// check if branch and if so set correct type, otherwise set 'isBranch' to false

		switch (opcode)
		{
		case I_CALL:

			addControlFlowInstruction(
				decodedInstruction,
				rawDataPtr,
				BranchType::CALL
			);
			break;

		case I_JMP_FAR:
			// #todo: add support for far jmp
			return false;

		case I_JMP:

			addControlFlowInstruction(
				decodedInstruction,
				rawDataPtr,
				BranchType::JMP
			);
			break;

		case I_JA:

			addControlFlowInstruction(
				decodedInstruction,
				rawDataPtr,
				BranchType::JA
			);
			break;

		case I_JAE:

			addControlFlowInstruction(
				decodedInstruction,
				rawDataPtr,
				BranchType::JAE
			);
			break;

		case I_JB:

			addControlFlowInstruction(
				decodedInstruction,
				rawDataPtr,
				BranchType::JB
			);
			break;

		case I_JBE:

			addControlFlowInstruction(
				decodedInstruction,
				rawDataPtr,
				BranchType::JBE
			);
			break;

		case I_JCXZ:

			// #todo: add support for JCXZ
			return false;
			break;

		case I_JECXZ:

			// #todo: add support for JECXZ
			return false;
			break;

		case I_JRCXZ:

			// #todo: add support for JRCXZ
			return false;
			break;

		case I_JG:

			addControlFlowInstruction(
				decodedInstruction,
				rawDataPtr,
				BranchType::JG
			);
			break;

		case I_JGE:

			addControlFlowInstruction(
				decodedInstruction,
				rawDataPtr,
				BranchType::JGE
			);
			break;

		case I_JL:

			addControlFlowInstruction(
				decodedInstruction,
				rawDataPtr,
				BranchType::JL
			);
			break;

		case I_JLE:

			addControlFlowInstruction(
				decodedInstruction,
				rawDataPtr,
				BranchType::JLE
			);
			break;

		case I_JNO:

			addControlFlowInstruction(
				decodedInstruction,
				rawDataPtr,
				BranchType::JNO
			);
			break;

		case I_JNP:

			addControlFlowInstruction(
				decodedInstruction,
				rawDataPtr,
				BranchType::JNP
			);
			break;

		case I_JNS:
			
			addControlFlowInstruction(
				decodedInstruction,
				rawDataPtr,
				BranchType::JNS
			);
			break;

		case I_JNZ:

			addControlFlowInstruction(
				decodedInstruction,
				rawDataPtr,
				BranchType::JNZ
			);
			break;

		case I_JO:

			addControlFlowInstruction(
				decodedInstruction,
				rawDataPtr,
				BranchType::JO
			);
			break;

		case I_JP:

			addControlFlowInstruction(
				decodedInstruction,
				rawDataPtr,
				BranchType::JP
			);
			break;

		case I_JS:

			addControlFlowInstruction(
				decodedInstruction,
				rawDataPtr,
				BranchType::JS
			);
			break;

		case I_JZ:

			addControlFlowInstruction(
				decodedInstruction,
				rawDataPtr,
				BranchType::JZ
			);
			break;

		case I_LOOP:
		case I_LOOPZ:
		case I_LOOPNZ:
			// #todo: add support for I_LOOP, I_LOOPZ, I_LOOPNZ
			// these branches aren't supported at the moment
			return false;

		default:

			addNormalInstruction(
				decodedInstruction,
				rawDataPtr
			);

			break;
		}
	}

	return true;
}

void InstructionNodesBuilder::addNormalInstruction(
	const _DInst	&inDecodedInstruction,
	uint8_t			*inData
)
{
	instructionList.emplace_back(
		new DecodedInstruction(
			inDecodedInstruction,
			inData
		)
	);
}

void InstructionNodesBuilder::addControlFlowInstruction(
	const _DInst	&inDecodedInstruction,
	uint8_t			*inData,
	BranchType		branchType
)
{
	uint32_t inJumpAddress = 0;

	// if relative branch, we must adjust the offset
	if (inDecodedInstruction.ops[0].type == O_PC)
	{
		inJumpAddress = (uint32_t)INSTRUCTION_GET_TARGET(&inDecodedInstruction);
	}
	// otherwise, just add it
	else
	{
		// #todo: this will be incorrect in some cases, e.g. switch case?
		addNormalInstruction(inDecodedInstruction, inData);
	}

	std::unique_ptr<BranchInstruction, InstructionDeleter> newBranch(
		new BranchInstruction(branchType)
	);

	// set virtual address in case there exist a branch to this branch
	newBranch->setVirtualAddress((uint32_t)inDecodedInstruction.addr);

	BranchRecord branchRecord;
	branchRecord.branchInstruction	= newBranch.get();
	branchRecord.jumpAddress		= inJumpAddress;

	branchRecords.push_back(std::move(branchRecord));
	instructionList.push_back(std::move(newBranch));
}

void InstructionNodesBuilder::linkLabels()
{
	for (auto &branchRecord : branchRecords)
	{
		auto *ins = getNodeWithVA(branchRecord.jumpAddress);
		if (ins)
		{
			branchRecord.branchInstruction->setBranchInstruction(ins);
		}
		else
		{
			branchRecord.branchInstruction->setBranchOffset(branchRecord.jumpAddress);
		}
	}
}

class Instruction* InstructionNodesBuilder::getNodeWithVA(uint32_t virtualAddress)
{
	for (auto &insNode : instructionList)
	{
		if (virtualAddress == insNode->getVirtualAddress())
		{
			return insNode.get();
		}
	}

	return nullptr;
}
