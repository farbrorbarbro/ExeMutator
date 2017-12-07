#pragma once

#include "distorm/distorm.h"
#include <vector>
#include "InstructionNodes/InstructionList.h"
#include "TextInstruction.h"


class CodeAnalysHelper
{
public:

	static bool decodeInstructions(
		const std::vector<uint8_t>	&inData,
		uint32_t					virtualAddress,
		std::vector<_DInst>			&out_decodedInstructions
	);

	static bool decodeInstructions(
		std::vector<uint8_t>	&inData,
		uint32_t				virtualAddress,
		InstructionList			&out_InstructionList
	);

	static void compileInstructions(
		InstructionList			&instructionList,
		uint32_t				virtualAddress,
		std::vector<uint8_t>	&outData
	);

	static std::string getString(
		const std::vector<uint8_t>	&inData,
		uint32_t					virtualAddress,
		InstructionPtr				&instruction
	);

	static std::string getString(
		const std::vector<uint8_t>	&inData,
		uint32_t					virtualAddress
	);

	static bool compileString(
		const std::string	&codeText,
		InstructionList		&instructionList
	);

	static bool compileTextInstructions(
		std::vector<TextInstruction>	&testInstructions,
		InstructionList					&instructionList
	);
};