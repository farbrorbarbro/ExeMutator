#include "TextInstructionsCompiler.h"
#include "CodeAnalysHelper.h"
#include "InstructionNodes\Instruction.h"


void TextInstructionsCompiler::addInstruction(TextInstruction &&newInstruction)
{
	textInstuctions.push_back(std::move(newInstruction));
}

bool TextInstructionsCompiler::compile()
{
	bool bCouldCompile = CodeAnalysHelper
		::compileTextInstructions(textInstuctions, compiledInstructions);

	return bCouldCompile;
}

uint32_t TextInstructionsCompiler::insertAt(
	InstructionList	&list,
	uint32_t		index,
	bool			bRemoveInstructionAtIndex
)
{
	if(bRemoveInstructionAtIndex)
	{
		// rebind all jumps to old instruction
		auto &oldInstruction = list[index];

		for (auto &insPtr : list)
		{
			if (insPtr->hasBranchToInstruction(oldInstruction.get()))
			{
				insPtr->setBranchInstruction(compiledInstructions[0].get());
			}
		}

		// remove old instruction
		list.erase(list.begin() + index);
	}


	const int32_t numOfInstructions = (int32_t)compiledInstructions.size();

	for (int32_t i = 0; i < numOfInstructions; ++i)
	{
		list.insert(list.begin() + index + i, std::move(compiledInstructions[i]));
	}

	compiledInstructions.clear();

	return numOfInstructions;
}

uint32_t TextInstructionsCompiler::getSize() const
{
	return (uint32_t)compiledInstructions.size();
}
