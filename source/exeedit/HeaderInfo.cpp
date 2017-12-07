#include "HeaderInfo.h"
#include <memory>
#include <string.h>
#include <string>
#include <iostream>
#include "PECoffHeaders.h"
#include "AlignmentHelper.h"


void HeaderInfo::initiate(
	uint8_t						*inFilePointer
	, struct ImageDosHeader		*inDosHeader
	, struct ImageNtHeaders		*inNtHeader
	, struct SectionHeader		*inSectionHeaders
)
{
	filePointer		= inFilePointer;

	dosHeader		= inDosHeader;
	ntHeader		= inNtHeader;
	sectionHeaders	= inSectionHeaders;
}

uint16_t HeaderInfo::getSectionNum() const
{
	return ntHeader->FileHeader.NumberOfSections;
}

struct SectionHeader* HeaderInfo::getSectionHeader(uint16_t sectionNr) const
{
	return sectionHeaders + sectionNr;
}

struct SectionHeader* HeaderInfo::getSectionHeader(const char *sectionName) const
{
	char nameBuffer[9];
	nameBuffer[8] = 0;

	for (uint32_t i = 0; i < getSectionNum(); ++i)
	{
		auto *currenctSection = sectionHeaders + i;

		char *rawSectionName = (char*)currenctSection->Name;
		memcpy(nameBuffer, rawSectionName, 8);

		if (strcmp(nameBuffer, sectionName) == 0)
		{
			return currenctSection;
		}
	}

	return nullptr;
}

uint8_t* HeaderInfo::getStartOfSection(uint16_t sectionNr) const
{
	auto *sectionHeader = getSectionHeader(sectionNr);
	return filePointer + sectionHeader->PointerToRawData;
}

uint8_t* HeaderInfo::getStartOfSection(const char *sectionName) const
{
	auto *sectionHeader = getSectionHeader(sectionName);
	if (sectionHeader)
	{
		return filePointer + sectionHeader->PointerToRawData;
	}

	return nullptr;
}

uint8_t* HeaderInfo::getStartOfFunction(const char *sectionName, uint32_t offset) const
{
	auto *sectionHeader = getSectionHeader(sectionName);
	if (sectionHeader)
	{
		auto *sectionData = filePointer + sectionHeader->PointerToRawData;
		auto sectionSize = sectionHeader->SizeOfRawData;

		auto *p2Function = sectionData + offset;

		return p2Function;
	}

	return nullptr;
}

uint32_t HeaderInfo::getSectionVA(const char *sectionName) const
{
	auto *sectionHeader = getSectionHeader(sectionName);
	if (sectionHeader)
	{
		return sectionHeader->VirtualAddress;
	}

	return 0;
}

uint32_t HeaderInfo::getSectionSize(const char *sectionName) const
{
	auto *sectionHeader = getSectionHeader(sectionName);
	if (sectionHeader)
	{
		return sectionHeader->SizeOfRawData;
	}

	return 0;
}

uint16_t HeaderInfo::getSectionIndex(struct SectionHeader *sectionHeader) const
{
	return sectionHeaders - sectionHeader;
}

uint32_t HeaderInfo::calculateFileAlignment(uint32_t currentOffset) const
{
	return AlignmentHelper::align(currentOffset, ntHeader->OptionalHeader.FileAlignment);
}

uint32_t HeaderInfo::calculateVirtualAdressAlignment(uint32_t nextVirtualAdress) const
{
	return AlignmentHelper::align(nextVirtualAdress, ntHeader->OptionalHeader.SectionAlignment);
}

void HeaderInfo::printSections() const
{
	std::string sectionNameString;

	char nameBuffer[9];
	nameBuffer[8] = 0;

	for (uint32_t i = 0; i < ntHeader->FileHeader.NumberOfSections; ++i)
	{
		auto *currenctSection = sectionHeaders + i;

		char *sectionName = (char*)currenctSection->Name;
		memcpy(nameBuffer, sectionName, 8);

		std::cout << nameBuffer << std::endl;
	}
}
