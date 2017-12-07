#include "FunctionParser.h"
#include "distorm/distorm.h"
#include <iostream>


FunctionParser::FunctionParser(const uint8_t *inData, uint32_t inDataMaxLenght)
	: data				{ inData }
	, dataMaxLenght		{ inDataMaxLenght}
	, dataOffset		{ 0 }
	, virtualAdress		{ 0 }
	, minParsingLength	{ 0 }
{
}

bool FunctionParser::tryParse()
{
	// initiate/reset values
	codeInfo			= { 0 };

	codeInfo.dt			= _DecodeType::Decode32Bits;
	codeInfo.features	= DF_NONE;
	
	dataOffset			= 0;
	minParsingLength	= 0;


	_DInst decodedInstruction;

	bool bFoundLastReturn = false;

	while(!bFoundLastReturn)
	{
		bool bDecodeWasSuccessful = parseNextInstruction(decodedInstruction);
		if (!bDecodeWasSuccessful)
		{
			return false;
		}

		decodedInstructions.push_back(decodedInstruction);

		// add raw data and increase data offset
		for (uint32_t i = 0; i < decodedInstruction.size; ++i)
		{
			dataBuffer.push_back(data[dataOffset]);
			++dataOffset;
		}


		if (META_GET_FC(decodedInstruction.meta) == FC_RET)
		{
			if (!isForcedParsing())
			{
				bFoundLastReturn = true;
			}
		}
		else if (META_GET_FC(decodedInstruction.meta) == FC_UNC_BRANCH)
		{
			_OffsetType absolute_target_address = 0;
			if (decodedInstruction.ops[0].type == O_PC) 
			{
				absolute_target_address = INSTRUCTION_GET_TARGET(&decodedInstruction);

				forceParsingUnitl(uint32_t(absolute_target_address - virtualAdress));
			}
			else
			{
				std::cout << "Warning: Unsupported jump found! Jump was ignored!" << std::endl;
			}
		}
		else if (META_GET_FC(decodedInstruction.meta) == FC_CND_BRANCH)
		{
			_OffsetType absolute_target_address = 0;
			if (decodedInstruction.ops[0].type == O_PC)
			{
				absolute_target_address = INSTRUCTION_GET_TARGET(&decodedInstruction);

				forceParsingUnitl(uint32_t(absolute_target_address - virtualAdress));
			}
			else
			{
				std::cout << "Warning: Unsupported jump found! Jump was ignored!" << std::endl;
			}
		}
	}

	return true;
}

std::vector<_DInst> FunctionParser::getDecodedInstructions() const
{
	return decodedInstructions;
}

std::vector<uint8_t> FunctionParser::getDataBuffer() const
{
	return dataBuffer;
}

std::string FunctionParser::toString() const
{
	std::string buildString;

	_CodeInfo codeInfo;
	codeInfo.code			= data;
	codeInfo.codeLen		= dataMaxLenght;
	codeInfo.codeOffset		= 0;
	codeInfo.dt				= _DecodeType::Decode32Bits;
	codeInfo.features		= DF_NONE;

	_DecodedInst inst;
	for (const auto &decodedInstruction : decodedInstructions)
	{
		if (decodedInstruction.flags == FLAG_NOT_DECODABLE)
		{
			// handle instruction error!
			break;
		}

		distorm_format(&codeInfo, &decodedInstruction, &inst);
		//printf("%s %s\n", inst.mnemonic.p, inst.operands.p);

		buildString.append((char*)inst.mnemonic.p);
		buildString.append(" ");
		buildString.append((char*)inst.operands.p);
		buildString.append("\n");
	}

	return buildString;
}

void FunctionParser::forceParsingUnitl(uint32_t inMinParsingLength)
{
	if (inMinParsingLength > minParsingLength)
	{
		minParsingLength = inMinParsingLength;
	}
}

bool FunctionParser::isForcedParsing() const
{
	return dataOffset <= minParsingLength;
}

bool FunctionParser::parseNextInstruction(_DInst &decodedInstruction)
{
	uint32_t decodedInstructionsCount = 0;

	codeInfo.code			= data + dataOffset;
	codeInfo.codeLen		= dataMaxLenght - dataOffset;
	codeInfo.codeOffset		= virtualAdress + dataOffset;

	_DecodeResult decodeResult = distorm_decompose(&codeInfo, &decodedInstruction, 1, &decodedInstructionsCount);

	if (decodedInstructionsCount != 1)
	{
		return false;
	}

	if (decodedInstruction.flags == FLAG_NOT_DECODABLE)
	{
		return false;
	}

	return true;
}
