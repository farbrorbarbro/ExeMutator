#include "CodeAnalysHelper.h"
#include "distorm/mnemonics.h"
#include "InstructionNodes/InstructionNodesBuilder.h"
#include "InstructionNodes/Instruction.h"
#include "asmtk/asmtk.h"


using namespace asmjit;
using namespace asmtk; 


bool CodeAnalysHelper::decodeInstructions(
	const std::vector<uint8_t>	&inData,
	uint32_t					virtualAddress,
	std::vector<_DInst>			&out_decodedInstructions
)
{
	out_decodedInstructions.clear();

	uint32_t dataSize = inData.size();

	if (dataSize < 1)
	{
		return false;
	}

	_CodeInfo codeInfo		= { 0 };
	codeInfo.code			= inData.data();
	codeInfo.codeLen		= dataSize;
	codeInfo.codeOffset		= virtualAddress;
	codeInfo.dt				= _DecodeType::Decode32Bits;
	codeInfo.features		= DF_NONE;


	out_decodedInstructions.resize(dataSize);

	uint32_t decodedInstructionsCount = 0;
	_DecodeResult decodeResult = distorm_decompose(&codeInfo, out_decodedInstructions.data(), dataSize, &decodedInstructionsCount);

	out_decodedInstructions.resize(decodedInstructionsCount);

	for (auto &decodedInstruction : out_decodedInstructions)
	{
		if (decodedInstruction.flags == FLAG_NOT_DECODABLE)
		{
			return false;
		}
	}

	return true;
}

bool CodeAnalysHelper::decodeInstructions(
	std::vector<uint8_t>	&inData,
	uint32_t				virtualAddress,
	InstructionList			&out_InstructionList
)
{
	InstructionNodesBuilder buildHelper(
		inData,
		virtualAddress,
		out_InstructionList
	);

	return buildHelper.build();
}

void CodeAnalysHelper::compileInstructions(
	InstructionList			&instructionList,
	uint32_t				virtualAddress,
	std::vector<uint8_t>	&outData
)
{
	outData.clear();

	// set virtual addresses
	for (auto &ins : instructionList)
	{
		ins->setVirtualAddress(virtualAddress);
		virtualAddress += ins->getInstructionSize();
	}

	// resolve jump addresses
	for (auto &ins : instructionList)
	{
		if (ins->isControlFlow())
		{
			ins->resolveLabelAddress();
		}
	}

	// output data
	for (auto &ins : instructionList)
	{
		auto instructionSize	= ins->getInstructionSize();
		auto *instructionData	= ins->getData();

		for (uint32_t i = 0; i < instructionSize; ++i)
		{
			outData.push_back(*(instructionData + i));
		}
	}
}

std::string CodeAnalysHelper::getString(
	const std::vector<uint8_t>	&inData,
	uint32_t					virtualAddress,
	InstructionPtr				&instruction
)
{
	std::string instructionText;

	auto *decodedInformation = instruction->getDecodedInstruction();
	if (decodedInformation)
	{
		_CodeInfo codeInfo		= { 0 };
		codeInfo.code			= inData.data();
		codeInfo.codeLen		= inData.size();
		codeInfo.codeOffset		= virtualAddress;
		codeInfo.dt				= _DecodeType::Decode32Bits;
		codeInfo.features		= DF_NONE;

		_DecodedInst inst;

		distorm_format(&codeInfo, decodedInformation, &inst);

		instructionText.append((char*)inst.mnemonic.p);
		instructionText.append(" ");
		instructionText.append((char*)inst.operands.p);
		instructionText.append("\n");
	}

	return instructionText;
}

std::string CodeAnalysHelper::getString(const std::vector<uint8_t> &inData, uint32_t virtualAddress)
{
	std::string returnString;

	std::vector<_DInst>	decodedInstructions;
	
	bool bCouldDecode = CodeAnalysHelper
		::decodeInstructions(inData, virtualAddress, decodedInstructions);

	if (!bCouldDecode)
	{
		returnString = "Failed to decode data!";
		return returnString;
	}

	_CodeInfo codeInfo = { 0 };
	codeInfo.code = inData.data();
	codeInfo.codeLen = inData.size();
	codeInfo.codeOffset = virtualAddress;
	codeInfo.dt = _DecodeType::Decode32Bits;
	codeInfo.features = DF_NONE;

	for (auto &decodedIns : decodedInstructions)
	{
		_DecodedInst inst;

		distorm_format(&codeInfo, &decodedIns, &inst);

		returnString.append((char*)inst.mnemonic.p);
		returnString.append(" ");
		returnString.append((char*)inst.operands.p);
		returnString.append("\n");
	}

	return returnString;
}

bool CodeAnalysHelper::compileString(
	const std::string	&codeText, 
	InstructionList		&instructionList
)
{
	if (codeText.size() < 1)
	{
		return false;
	}

	CodeInfo codeInfo(ArchInfo::kTypeX86);
	CodeHolder code;
	code.init(codeInfo);

	X86Assembler a(&code);
	AsmParser p(&a);

	Error err = p.parse(
		codeText.c_str()
	);

	if (err) 
	{
		return false;
	}

	// If we are done, you must detach the Assembler from CodeHolder or sync
	// it, so its internal state and position is synced with CodeHolder.
	code.sync();

	// Now you can print the code, which is stored in the first section (.text).
	CodeBuffer& buffer = code.getSectionEntry(0)->getBuffer();

	std::vector<uint8_t> bytes;
	bytes.resize(buffer.getLength());

	memcpy(bytes.data(), buffer.getData(), buffer.getLength());

	return decodeInstructions(bytes, 0, instructionList);
}

bool CodeAnalysHelper::compileTextInstructions(
	std::vector<TextInstruction> &testInstructions, 
	InstructionList &instructionList
)
{
	std::string asmCode;

	for (auto &textInstruction : testInstructions)
	{
		asmCode += textInstruction.getInstuctionAsString();
		asmCode += "\n";
	}

	return compileString(asmCode, instructionList);
}

