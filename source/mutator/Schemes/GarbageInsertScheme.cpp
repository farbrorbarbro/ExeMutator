#include "GarbageInsertScheme.h"
#include "InstructionNodes/InstructionList.h"
#include "CodeAnalysHelper.h"
#include "Random/RandomGenerator.h"
#include "InstructionNodes/NopInstruction.h"
#include <memory>
#include "InstructionNodes/RegisterDefinitions.h"
#include "InstructionNodes/PushInstruction.h"
#include "InstructionNodes/PopInstruction.h"
#include "InstructionNodes/InstructionPtr.h"
#include "MutatorException.h"


void GarbageInsertScheme::applyScheme(
	std::vector<uint8_t>	&inData, 
	uint32_t				virtualAddress, 
	uint32_t				newVirtualAddress
)
{
	InstructionList instructionList;

	if (!CodeAnalysHelper::decodeInstructions(inData, virtualAddress, instructionList))
	{
		throw MutatorException("Failed to decode instructions in garbage insertion scheme!");
	}

	// iterate through instructions, ignore last instruction
	// don't insert garbage after return statement
	for (uint32_t i = 0; i < instructionList.size() - 1;)
	{
		uint32_t instructionsLeft = instructionList.size() - (i + 1);

		auto blockSize		= generateBlockSize(instructionsLeft);
		auto garbageLength	= insertGarbage(instructionList, i);

		i += blockSize + garbageLength;
	}

	// convert back to bytes
	CodeAnalysHelper::compileInstructions(instructionList, newVirtualAddress, inData);
}

uint32_t GarbageInsertScheme::getMinBlock() const
{
	return minBlock;
}

void GarbageInsertScheme::setMinBlock(uint32_t val)
{
	if (val < 1)
	{
		val = 1;
	}
	else
	{
		minBlock = val;
	}
}

uint32_t GarbageInsertScheme::getMaxBlock() const
{
	return maxBlock;
}

void GarbageInsertScheme::setMaxBlock(uint32_t val)
{
	if (val < 1)
	{
		maxBlock = 1;
	}
	else
	{
		maxBlock = val;
	}
}

uint32_t GarbageInsertScheme::getMaxRandomBlockSize() const
{
	return maxRandomBlockSize;
}

void GarbageInsertScheme::setMaxRandomBlockSize(uint32_t val)
{
	if (val < 1)
	{
		maxRandomBlockSize = 1;
	}
	else
	{
		maxRandomBlockSize = val;
	}
}

uint32_t GarbageInsertScheme::generateBlockSize(uint32_t max)
{
	auto result = minBlock + randomGenerator->generateNumberRange(maxBlock + 1 - minBlock);
	return result > max ? max : result;
}

uint32_t GarbageInsertScheme::insertGarbage(
	InstructionList	&instructionList, 
	uint32_t		startIndex
)
{
	// #todo: implement add more instructions, e.g. mov, pushf, popf...

	uint32_t instructionCount = randomGenerator->generateNumberRange(maxRandomBlockSize) + 1;

	auto instructionsLeft = instructionCount;

	while (instructionsLeft > 0)
	{
		auto garbageType = randomGenerator->generateNumberRange(2);

		if (garbageType == 0 && instructionsLeft >= 2)
		{
			auto registerType = getRandom32bitRegister(randomGenerator);

			InstructionPtr pushInstruction	(new PushInstruction(registerType));
			InstructionPtr popInstruction	(new PopInstruction(registerType));

			// insert reverse order to get right order
			instructionList.insert(instructionList.begin() + startIndex, std::move(popInstruction));
			instructionList.insert(instructionList.begin() + startIndex, std::move(pushInstruction));

			instructionsLeft -= 2;

			continue;
		}
		

		// generate and add nop instruction
		InstructionPtr nopInstruction	(new NopInstruction());
		instructionList.insert(instructionList.begin() + startIndex, std::move(nopInstruction));

		--instructionsLeft;
	}

	return instructionCount;
}
