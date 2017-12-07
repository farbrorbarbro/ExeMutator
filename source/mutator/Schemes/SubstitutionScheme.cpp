#include "SubstitutionScheme.h"
#include "CodeAnalysHelper.h"
#include "InstructionNodes/Instruction.h"
#include "TextInstruction.h"
#include "InstructionNodes/RegisterDefinitions.h"
#include "Random/RandomGenerator.h"
#include "TextInstructionsCompiler.h"
#include <iostream>
#include "MutatorException.h"


void SubstitutionScheme::applyScheme(
	std::vector<uint8_t>	&inData,
	uint32_t				virtualAddress,
	uint32_t				newVirtualAddress
)
{
	InstructionList instructionList;

	if (!CodeAnalysHelper::decodeInstructions(inData, virtualAddress, instructionList))
	{
		throw MutatorException("Failed to decode instructions in substitution scheme!");
	}


	for (uint32_t i = 0; i < instructionList.size() - 1;)
	{
		auto &instruction = instructionList[i];

		// control flow is not supported at the moment since it has no decoded info

		auto asmText = CodeAnalysHelper::getString(inData, virtualAddress, instruction);


		TextInstruction textInstruction = TextInstruction::parseInstruction(asmText);

		if (textInstruction.getInstructionName() == "PUSH")
		{
			i += substitutePushPop(instructionList, i, textInstruction, true);
		}
		else if (textInstruction.getInstructionName() == "POP")
		{
			i += substitutePushPop(instructionList, i, textInstruction, false);
		}
		else if (textInstruction.getInstructionName() == "MOV")
		{
			i += substituteMov(instructionList, i, textInstruction);
		}
		// #todo: add support for more instructions to substitute
		else
		{
			++i;
		}
	}


	// convert back to bytes
	CodeAnalysHelper::compileInstructions(instructionList, newVirtualAddress, inData);
}

uint32_t SubstitutionScheme::substitutePushPop(
	InstructionList					&instructionList,
	uint32_t						index,
	const class TextInstruction		&instruction,
	bool							bIsPush
)
{
	const std::string& originalOperator = instruction.getOperator1();

	if (originalOperator == "ESP" || originalOperator == "EBP")
	{
		return 1;
	}

	RegisterDefinitions random32bitRegister = RegisterDefinitions::EAX;
	const char* randomRegisterString = nullptr;

	do
	{
		random32bitRegister = getRandom32bitRegister(randomGenerator);
		randomRegisterString = getAsString(random32bitRegister);

	} while (originalOperator == randomRegisterString);


	TextInstructionsCompiler helpCompiler;

	generateRegisterSwap(helpCompiler, originalOperator.c_str(), randomRegisterString);

	if (bIsPush)
	{
		helpCompiler.addInstruction(TextInstruction("PUSH", randomRegisterString));
	}
	else
	{
		helpCompiler.addInstruction(TextInstruction("POP", randomRegisterString));
	}

	generateRegisterSwap(helpCompiler, originalOperator.c_str(), randomRegisterString);


	bool bCouldCompile = helpCompiler.compile();

	if (!bCouldCompile)
	{
		if (bIsPush)
		{
			std::cout << "Warning: Failed to compile instructions! Substitution for PUSH instruction will be ignored!" << std::endl;
		}
		else
		{
			std::cout << "Warning: Failed to compile instructions! Substitution for POP instruction will be ignored!" << std::endl;
		}

		return 1;
	}

	uint32_t instructionCount = helpCompiler.insertAt(instructionList, index, true);
	return instructionCount;
}

uint32_t SubstitutionScheme::substituteMov(
	InstructionList					&instructionList,
	uint32_t						index,
	const class TextInstruction		&instruction
)
{
	const std::string& operator1 = instruction.getOperator1();
	const std::string& operator2 = instruction.getOperator2();

	if (operator1 == "ESP" || operator1 == "EBP")
	{
		return 1;
	}


	RegisterDefinitions random32bitRegister	= RegisterDefinitions::EAX;
	const char* randomRegisterString		= nullptr;

	do
	{
		random32bitRegister		= getRandom32bitRegister(randomGenerator);
		randomRegisterString	= getAsString(random32bitRegister);

	} while (
			operator1 == randomRegisterString
		||	operator2 == randomRegisterString
	);


	TextInstructionsCompiler helpCompiler;

	// lazy check if registers
	if(operator1.size() == 3 && operator2.size() == 3)
	{
		uint32_t substitutionMethod = randomGenerator->generateNumberRange(3);

		if (substitutionMethod == 0)
		{
			helpCompiler.addInstruction(TextInstruction("PUSH", operator2.c_str()));
			helpCompiler.addInstruction(TextInstruction("POP", operator1.c_str()));
		}
		else if (substitutionMethod == 1)
		{
			helpCompiler.addInstruction(TextInstruction("PUSH", randomRegisterString));
			helpCompiler.addInstruction(TextInstruction("MOV", randomRegisterString, operator2.c_str()));
			helpCompiler.addInstruction(TextInstruction("MOV", operator1.c_str(), randomRegisterString));
			helpCompiler.addInstruction(TextInstruction("POP", randomRegisterString));
		}
		else if (substitutionMethod == 2)
		{
			helpCompiler.addInstruction(TextInstruction("PUSH", randomRegisterString));
			helpCompiler.addInstruction(TextInstruction("PUSH", operator2.c_str()));
			helpCompiler.addInstruction(TextInstruction("POP", randomRegisterString));
			helpCompiler.addInstruction(TextInstruction("MOV", operator1.c_str(), randomRegisterString));
			helpCompiler.addInstruction(TextInstruction("POP", randomRegisterString));
		}
	}
	else
	{
		// #todo: implement support if not registers as operands
		return 1;
	}
	

	bool bCouldCompile = helpCompiler.compile();

	if (!bCouldCompile)
	{
		std::cout << "Warning: Failed to compile instructions! Substitution for MOV instruction will be ignored!" << std::endl;
		return 1;
	}

	uint32_t instructionCount = helpCompiler.insertAt(instructionList, index, true);
	return instructionCount;
}

void SubstitutionScheme::generateRegisterSwap(
	class TextInstructionsCompiler	&helpCompiler,
	const char						*reg1,
	const char						*reg2
)
{
	bool bSwapRegisters = randomGenerator->generateBool();

	const char *regA = bSwapRegisters ? reg2 : reg1;
	const char *regB = bSwapRegisters ? reg1 : reg2;

	uint32_t generatedNum = randomGenerator->generateNumberRange(2);
	if (generatedNum == 0)
	{
		helpCompiler.addInstruction(TextInstruction("PUSH", regA));
		helpCompiler.addInstruction(TextInstruction("PUSH", regB));
		helpCompiler.addInstruction(TextInstruction("POP", regA));
		helpCompiler.addInstruction(TextInstruction("POP", regB));
	}
	else if(generatedNum == 1)
	{
		helpCompiler.addInstruction(TextInstruction("PUSH", regA));
		helpCompiler.addInstruction(TextInstruction("MOV", regA, regB));
		helpCompiler.addInstruction(TextInstruction("POP", regB));
	}
}