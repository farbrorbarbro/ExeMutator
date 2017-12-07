#pragma once

#include <vector>


class Section
{
public:

	Section(
		const char	*inSectionName, 
		uint32_t	inSectionSize, 
		uint32_t	inVirtualAddress
	);
	
	Section(
		const char	*inSectionName, 
		uint32_t	inSectionSize, 
		uint32_t	inVirtualAddress, 
		uint8_t		inAlignment
	);
	
	uint32_t addFunction(const class AsmFunction &newFunction);

	const std::vector<uint8_t>& getData() const;

	const char* getSectionName() const { return sectionName; }

	uint32_t calculateSizeWithAlignment(uint32_t alignment) const;
	uint32_t getTotalCodeLength() const;


	uint32_t getNextOffset() const;
	uint32_t getVirtualAddress() const;
	uint32_t getNextVirtualAdress() const;


private:

	void setSectionName(const char *inSectionName);
	void addPadding();

private:

	uint32_t				sectionSize = 0;
	uint32_t				virtualAddress = 0;

	char					sectionName[9] = {0};

	uint8_t					alignment = 8;
	std::vector<uint8_t>	sectionDataBuffer;

	uint32_t				totalCodeLength = 0;
};
