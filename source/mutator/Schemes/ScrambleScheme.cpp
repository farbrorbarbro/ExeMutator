#include "ScrambleScheme.h"
#include "InstructionNodes/InstructionList.h"
#include "CodeAnalysHelper.h"
#include "Random/RandomGenerator.h"
#include "InstructionNodes/BranchInstruction.h"
#include "ScrambleBlock.h"
#include "MutatorException.h"


void ScrambleScheme::applyScheme(
	std::vector<uint8_t>	&inData, 
	uint32_t				virtualAddress, 
	uint32_t				newVirtualAddress
)
{
	if (minBlock > maxBlock)
	{
		throw MutatorException("Incorrect settings found in scramble insertion scheme!");
	}

	InstructionList instructionList;

	if (!CodeAnalysHelper::decodeInstructions(inData, virtualAddress, instructionList))
	{
		throw MutatorException("Failed to decode instructions in scramble scheme!");
	}
	
	// create jump to first instruction
	std::unique_ptr<BranchInstruction, InstructionDeleter> 
		firstInstruction(new BranchInstruction(BranchType::JMP));

	firstInstruction->setBranchInstruction(instructionList[0].get());


	// create blocks
	std::vector<std::unique_ptr<ScrambleBlock>> blocks;
	
	uint32_t instructionNum		= instructionList.size();
	ScrambleBlock *lastBlock	= nullptr;

	for (uint32_t i = 0; i < instructionNum;)
	{
		auto newBlockSize = generateBlockSize(instructionNum - i);
		auto scrambleBlock = std::make_unique<ScrambleBlock>();

		for (uint32_t j = i; j < i + newBlockSize; ++j)
		{
			scrambleBlock->addInstruction(std::move(instructionList[j]));
		}

		if (lastBlock)
		{
			std::unique_ptr<BranchInstruction, InstructionDeleter> 
				jumpIns(new BranchInstruction(BranchType::JMP));

			jumpIns->setBranchInstruction(scrambleBlock->getInstructions()[0].get());

			lastBlock->addInstruction(std::move(jumpIns));
		}

		lastBlock = scrambleBlock.get();
		blocks.push_back(std::move(scrambleBlock));

		i += newBlockSize;
	}

	instructionList.clear();


	// shuffle blocks
	std::vector<std::unique_ptr<ScrambleBlock>> scrambledBlocks;
	while (blocks.size() > 0)
	{
		auto nextIndex = randomGenerator->generateNumberRange(blocks.size());
		scrambledBlocks.push_back(std::move(blocks[nextIndex]));

		blocks.erase(blocks.begin() + nextIndex);
	}
	
	blocks.clear();

	
	// add shuffled instructions back
	instructionList.push_back(std::move(firstInstruction));

	for (auto &block : scrambledBlocks)
	{
		auto &instructions = block->getInstructions();

		for (auto &ins : instructions)
		{
			instructionList.push_back(std::move(ins));
		}
	}

	scrambledBlocks.clear();


	// convert back to bytes
	CodeAnalysHelper::compileInstructions(instructionList, newVirtualAddress, inData);
}

uint32_t ScrambleScheme::getMinBlock() const
{
	return minBlock;
}

void ScrambleScheme::setMinBlock(uint32_t val)
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

uint32_t ScrambleScheme::getMaxBlock() const
{
	return maxBlock;
}

void ScrambleScheme::setMaxBlock(uint32_t val)
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

uint32_t ScrambleScheme::generateBlockSize(uint32_t max)
{
	auto result = minBlock + randomGenerator->generateNumberRange(maxBlock + 1 - minBlock);
	return result > max ? max : result;
}