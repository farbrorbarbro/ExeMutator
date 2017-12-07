#include "ExeEdit.h"
#include <fstream>
#include <string>
#include <iostream>
#include "PECoffHeaders.h"
#include "AlignmentHelper.h"
#include "ExeEditException.h"


ExeEdit::~ExeEdit()
{
}

void ExeEdit::loadBinaryFile(const char *filepath)
{
	reset();

	if (!readAllBytes(filepath))
	{
		throw ExeEditException("Failed to read file!");
	}

	resolveSectionPointers(
		rawFile.get()
		, dosHeader
		, ntHeader
		, sectionHeaders
	);

	headerInfo.initiate(
		rawFile.get()
		, dosHeader
		, ntHeader
		, sectionHeaders
	);

	calculateNextVirtualAddress();

	headerInfo.printSections();
}

void ExeEdit::writeBinaryFile(const char *filepath) const
{
	uint32_t newSectionsSize		= 0;
	uint32_t newSectionHeadersSize	= 0;
	uint32_t newCodeLenght			= 0;

	for (auto &newSection : newSections)
	{
		auto sectionSize = newSection
			.calculateSizeWithAlignment(ntHeader->OptionalHeader.FileAlignment);

		// size with padding, plus section header
		newSectionsSize			+= sectionSize;
		newSectionHeadersSize	+= sizeof(SectionHeader);
		
		// assume all new section are code for now
		newCodeLenght			+= sectionSize;
	}

	newSectionHeadersSize = AlignmentHelper::align(newSectionHeadersSize, ntHeader->OptionalHeader.FileAlignment);

	uint32_t newFileSize = fileLength + newSectionsSize + newSectionHeadersSize;

	std::unique_ptr<uint8_t> newFile(new uint8_t[newFileSize]);

	const auto *originalFile = rawFile.get();
	auto *newFilePointer = newFile.get();

	// zero memory for file
	memset(newFilePointer, 0, newFileSize);

	const auto *afterLastOriginalSection = (uint8_t*)(sectionHeaders + headerInfo.getSectionNum());

	memcpy(newFilePointer, originalFile, afterLastOriginalSection - originalFile);


	ImageDosHeader	*newDosHeader;
	ImageNtHeaders	*newNtHeader;
	SectionHeader	*newSectionHeaders;

	resolveSectionPointers(
		newFilePointer
		, newDosHeader
		, newNtHeader
		, newSectionHeaders
	);

	HeaderInfo newHeaderInfo;

	newHeaderInfo.initiate(
		newFile.get()
		, newDosHeader
		, newNtHeader
		, newSectionHeaders
	);


	// update size of code field
	newNtHeader->OptionalHeader.SizeOfCode += newCodeLenght;


	auto *writeHead = (uint8_t*)(newSectionHeaders + newHeaderInfo.getSectionNum());

	for (auto &newSection : newSections)
	{
		SectionHeader newHeader;
		memcpy(newHeader.Name, newSection.getSectionName(), sizeof(newHeader.Name));
		
		newHeader.Misc.VirtualSize		= newSection.getData().size();

		newHeader.VirtualAddress		= newSection.getVirtualAddress();

		newHeader.SizeOfRawData			= newSection.calculateSizeWithAlignment(newNtHeader->OptionalHeader.FileAlignment);
		
		newHeader.PointerToRawData		= 0;

		newHeader.PointerToRelocations	= 0;
		newHeader.PointerToLinenumbers	= 0;
		newHeader.NumberOfRelocations	= 0;
		newHeader.NumberOfLinenumbers	= 0;

		// assume it's a code section
		newHeader.Characteristics		= (uint32_t)SectionCharacteristics::IMAGE_SCN_CNT_CODE 
										| (uint32_t)SectionCharacteristics::IMAGE_SCN_MEM_READ 
										| (uint32_t)SectionCharacteristics::IMAGE_SCN_MEM_EXECUTE;


		// write section to section array
		memcpy(writeHead, &newHeader, sizeof(newHeader));
		writeHead += sizeof(newHeader);
	}


	// update SizeOfImage property
	newNtHeader->OptionalHeader.SizeOfImage = nextVirtualAddress;


	// update SizeOfHeaders property
	auto totalHeadersSize = writeHead - newFilePointer;
	totalHeadersSize = newHeaderInfo.calculateFileAlignment(totalHeadersSize);
	newNtHeader->OptionalHeader.SizeOfHeaders = totalHeadersSize;

	
	// write section data
	newNtHeader->FileHeader.NumberOfSections += (uint16_t) newSections.size();

	for (uint32_t i = 0; i < newHeaderInfo.getSectionNum(); ++i)
	{
		// calculate file alignment
		auto currentOffset = writeHead - newFilePointer;
		writeHead = newFilePointer + newHeaderInfo.calculateFileAlignment(currentOffset);


		auto *currenctSection = newSectionHeaders + i;

		currenctSection->PointerToRawData = writeHead - newFilePointer;

		// is original section
		if (i < headerInfo.getSectionNum())
		{
			auto *startOfSection = headerInfo.getStartOfSection(i);

			memcpy(writeHead, startOfSection, currenctSection->SizeOfRawData);
			
		}
		// is added section
		else
		{
			auto &newSection = newSections[i - headerInfo.getSectionNum()];
			memcpy(writeHead, newSection.getData().data(), currenctSection->SizeOfRawData);
		}

		writeHead += currenctSection->SizeOfRawData;
	}


	auto currentOffset = writeHead - newFilePointer;
	writeHead = newFilePointer + newHeaderInfo.calculateFileAlignment(currentOffset);
	

	auto length = writeHead - newFilePointer;

	// note: newFileSize >= length should always be valid

	// override selected functions
	for (auto &functionOverride : functionOverrideRecords)
	{
		auto oldFunctionVA = newHeaderInfo.getSectionVA(functionOverride.sectionName) 
			+ functionOverride.offset;

		auto newFunctionVA = newHeaderInfo.getSectionVA(functionOverride.newSectionName)
			+ functionOverride.newSectionOffset;

		uint8_t jumpInstruction[5];
		jumpInstruction[0] = 0xE9;

		uint32_t offset = newFunctionVA - oldFunctionVA - sizeof(jumpInstruction);
		
		*(uint32_t*)(&jumpInstruction[1]) = offset;


		auto *oldFunctionPtr = newHeaderInfo.getStartOfFunction(
			functionOverride.sectionName, functionOverride.offset);

		memcpy(oldFunctionPtr, jumpInstruction, sizeof(jumpInstruction));
		
		memset(
			oldFunctionPtr + sizeof(jumpInstruction)
			, 0x90
			, functionOverride.newFunction.getOriginalSize() - sizeof(jumpInstruction)
		);
	}


	std::ofstream outFile(filepath, std::ios::binary);
	outFile.write((char*)newFilePointer, length);
	outFile.close();
}

void ExeEdit::reset()
{
	rawFile.reset();
	fileLength = 0;

	dosHeader = nullptr;
	ntHeader = nullptr;
	sectionHeaders = nullptr;

	newSections.clear();
	functionOverrideRecords.clear();
}

AsmFunction ExeEdit::parseFunctionAt(
	const char *sectionName
	, uint32_t offset
)
{
	// #todo: add relocation data to assembler function and inject it here

	AsmFunction asmFunction;
	asmFunction.setSectionName(sectionName);
	asmFunction.setOffset(offset);

	auto *functionPtr = headerInfo.getStartOfFunction(sectionName, offset);
	if (functionPtr)
	{
		auto sectionSize	= headerInfo.getSectionSize(sectionName);
		auto virtualAdress	= headerInfo.getSectionVA(sectionName);

		asmFunction.tryParse(
			functionPtr
			, sectionSize - offset
			, virtualAdress + offset
		);
	}

	return asmFunction;
}

Section& ExeEdit::createNewSection(const char *sectionName, uint32_t sectionSize)
{
	Section newSection(sectionName, sectionSize, nextVirtualAddress);

	nextVirtualAddress += sectionSize;
	calculateNextVirtualAddress();

	newSections.push_back(std::move(newSection));
	return newSections.back();
}

void ExeEdit::overrideFunctionAt(
	const Section		&newSection
	, uint32_t			newSectionOffset
	, const AsmFunction	&newFunction
)
{
	// if function is shorter than 5 bytes we can't add a 5 byte JMP safely
	if (newFunction.getOriginalSize() < 5)
	{
		// #todo: use short jmp if possible?
		// return;

		std::cout	<< "Warning: overridden function ( " << newFunction.getOffset() << " )was less than 5 bytes!" << std::endl
					<< "\tJump was added but might be unsafe!" << std::endl;
	}

	FunctionOverrideRecord overrideRecord;

	overrideRecord.sectionName		= newFunction.getSectionName();
	overrideRecord.offset			= newFunction.getOffset();
	overrideRecord.newSectionName	= newSection.getSectionName();
	overrideRecord.newSectionOffset = newSectionOffset;
	overrideRecord.newFunction		= newFunction;

	functionOverrideRecords.push_back(std::move(overrideRecord));
}

bool ExeEdit::readAllBytes(const char *filepath)
{
	// open file
	std::ifstream infile(filepath, std::ios::binary);

	if (!infile.is_open())
	{
		return false;
	}

	//get length of file
	infile.seekg(0, infile.end);
	fileLength = (uint32_t)infile.tellg();

	if (fileLength < 1)
	{
		return false;
	}

	// allocate memory for file
	rawFile.reset(new uint8_t[fileLength]);

	infile.seekg(0, infile.beg);

	//read file
	infile.read((char*)rawFile.get(), fileLength);

	infile.close();

	return true;
}

void ExeEdit::resolveSectionPointers(
	const uint8_t			*fileStart
	, struct ImageDosHeader	*&out_dosHeader
	, struct ImageNtHeaders	*&out_ntHeaders
	, struct SectionHeader	*&out_sectionHeaders
)
{
	out_dosHeader = (ImageDosHeader*)fileStart;
	out_ntHeaders = (ImageNtHeaders*)(fileStart + out_dosHeader->e_lfanew);

	auto *fileHeader = (ImageFileHeader*)&out_ntHeaders->FileHeader;
	auto *optionalHeader = (ImageOptionalHeader*)&out_ntHeaders->OptionalHeader;

	out_sectionHeaders = (SectionHeader*)
		((uint8_t*)optionalHeader + fileHeader->SizeOfOptionalHeader);
}

void ExeEdit::calculateNextVirtualAddress()
{
	if(nextVirtualAddress == 0)
	{
		auto *lastSectionHeader = sectionHeaders + headerInfo.getSectionNum() - 1;

		nextVirtualAddress = lastSectionHeader->VirtualAddress;
		nextVirtualAddress += lastSectionHeader->Misc.VirtualSize;
	}

	nextVirtualAddress = headerInfo.calculateVirtualAdressAlignment(nextVirtualAddress);
}