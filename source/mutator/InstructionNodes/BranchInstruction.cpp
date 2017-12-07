#include "BranchInstruction.h"


BranchInstruction::BranchInstruction(BranchType inBranchType)
	: branchType { inBranchType }
{
	initiateInstruction();
}

bool BranchInstruction::isControlFlow() const
{
	return true;
}

void BranchInstruction::resolveLabelAddress()
{
	auto firstOffsetByteIndex = instructionLength - 4;

	auto jumpOffset = label.calculateRelativeJump(getVirtualAddress(), instructionLength);

	*((int32_t*)&instructionBytes[firstOffsetByteIndex]) = jumpOffset;
}

void BranchInstruction::setBranchInstruction(Instruction *instruction)
{
	label.setInstruction(instruction);
}

void BranchInstruction::setBranchOffset(uint32_t jumpOffset)
{
	label.setJumpOffset(jumpOffset);
}

bool BranchInstruction::hasBranchToInstruction(Instruction *instruction) const
{
	return label.getInstruction() == instruction;
}

void BranchInstruction::initiateInstruction()
{
	switch (branchType)
	{
	case BranchType::CALL:
		instructionBytes[0] = 0xE8;
		instructionLength = 5;
		break;

	case BranchType::JMP:
		instructionBytes[0]	= 0xE9;
		instructionLength	= 5;
		break;

	case BranchType::JA:
		instructionBytes[0] = 0x0F;
		instructionBytes[1] = 0x87;
		instructionLength = 6;
		break;

	case BranchType::JAE:
		instructionBytes[0] = 0x0F;
		instructionBytes[1] = 0x83;
		instructionLength = 6;
		break;

	case BranchType::JB:
		instructionBytes[0] = 0x0F;
		instructionBytes[1] = 0x82;
		instructionLength = 6;
		break;

	case BranchType::JBE:
		instructionBytes[0] = 0x0F;
		instructionBytes[1] = 0x86;
		instructionLength = 6;
		break;

	case BranchType::JG:
		instructionBytes[0] = 0x0F;
		instructionBytes[1] = 0x8F;
		instructionLength = 6;
		break;

	case BranchType::JGE:
		instructionBytes[0] = 0x0F;
		instructionBytes[1] = 0x8D;
		instructionLength = 6;
		break;

	case BranchType::JL:
		instructionBytes[0] = 0x0F;
		instructionBytes[1] = 0x8C;
		instructionLength = 6;
		break;

	case BranchType::JLE:
		instructionBytes[0] = 0x0F;
		instructionBytes[1] = 0x8E;
		instructionLength = 6;
		break;

	case BranchType::JNO:
		instructionBytes[0] = 0x0F;
		instructionBytes[1] = 0x81;
		instructionLength = 6;
		break;

	case BranchType::JNP:
		instructionBytes[0] = 0x0F;
		instructionBytes[1] = 0x8B;
		instructionLength = 6;
		break;

	case BranchType::JNS:
		instructionBytes[0] = 0x0F;
		instructionBytes[1] = 0x89;
		instructionLength = 6;
		break;

	case BranchType::JNZ:
		instructionBytes[0] = 0x0F;
		instructionBytes[1] = 0x85;
		instructionLength = 6;
		break;

	case BranchType::JO:
		instructionBytes[0] = 0x0F;
		instructionBytes[1] = 0x80;
		instructionLength = 6;
		break;

	case BranchType::JP:
		instructionBytes[0] = 0x0F;
		instructionBytes[1] = 0x8A;
		instructionLength = 6;
		break;

	case BranchType::JS:
		instructionBytes[0] = 0x0F;
		instructionBytes[1] = 0x88;
		instructionLength = 6;
		break;

	case BranchType::JZ:
		instructionBytes[0] = 0x0F;
		instructionBytes[1] = 0x84;
		instructionLength = 6;

		break;

	default:
		break;

	}
}
