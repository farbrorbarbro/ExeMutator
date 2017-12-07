#pragma once

#include <stdint.h>


class HeaderInfo
{
public:

	HeaderInfo() {}
	
	void initiate(
		uint8_t						*inFilePointer
		, struct ImageDosHeader		*inDosHeader
		, struct ImageNtHeaders		*inNtHeader
		, struct SectionHeader		*inSectionHeaders
	);

	uint16_t getSectionNum() const;

	struct SectionHeader* getSectionHeader(uint16_t sectionNr) const;
	struct SectionHeader* getSectionHeader(const char *sectionName) const;

	uint8_t* getStartOfSection(uint16_t sectionNr) const;
	uint8_t* getStartOfSection(const char *sectionName) const;

	uint8_t* getStartOfFunction(
		const char *sectionName
		, uint32_t offset
	) const;

	uint32_t getSectionVA(const char *sectionName) const;

	uint32_t getSectionSize(const char *sectionName) const;
	uint16_t getSectionIndex(struct SectionHeader *sectionHeader) const;

	uint32_t calculateFileAlignment(uint32_t currentOffset) const;
	uint32_t calculateVirtualAdressAlignment(uint32_t nextVirtualAdress) const;

	void printSections() const;

private:

	uint8_t						*filePointer	= nullptr;

	struct ImageDosHeader		*dosHeader		= nullptr;
	struct ImageNtHeaders		*ntHeader		= nullptr;
	struct SectionHeader		*sectionHeaders	= nullptr;
};

