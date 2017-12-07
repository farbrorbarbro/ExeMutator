#include "Section.h"
#include "AsmFunction.h"
#include "ExeEditException.h"


Section::Section(
	const char	*inSectionName,
	uint32_t	inSectionSize,
	uint32_t	inVirtualAddress
)
	: sectionSize{ inSectionSize }
	, virtualAddress{ inVirtualAddress }
{
	setSectionName(inSectionName);
}

Section::Section(
	const char	*inSectionName,
	uint32_t	inSectionSize,
	uint32_t	inVirtualAddress,
	uint8_t		inAlignment
)
	: sectionSize		{ inSectionSize }
	, virtualAddress	{ inVirtualAddress }
	, alignment			{ inAlignment }
{
	setSectionName(inSectionName);
}

uint32_t Section::addFunction(const class AsmFunction &newFunction)
{
	auto newIndex = sectionDataBuffer.size();

	if(newIndex + newFunction.getFunctionLength() < sectionSize)
	{
		auto &data = newFunction.getData();

		sectionDataBuffer.insert(std::end(sectionDataBuffer), std::begin(data), std::end(data));

		totalCodeLength += data.size();

		addPadding();

		return newIndex;
	}
	
	throw ExeEditException("Function couldn't be added to section, section was too small!");
}

const std::vector<uint8_t>& Section::getData() const
{
	return sectionDataBuffer;
}

uint32_t Section::calculateSizeWithAlignment(uint32_t alignment) const
{
	uint32_t dataSize = sectionDataBuffer.size();
	
	auto rest = dataSize % alignment;
	auto padding = rest != 0 ? alignment - rest : 0;

	auto sizeWithPadding = dataSize + padding;
	return sizeWithPadding;
}

uint32_t Section::getTotalCodeLength() const
{
	return totalCodeLength;
}

uint32_t Section::getNextOffset() const
{
	return sectionDataBuffer.size();
}

uint32_t Section::getVirtualAddress() const
{
	return virtualAddress;
}

uint32_t Section::getNextVirtualAdress() const
{
	return getVirtualAddress() + getNextOffset();
}

void Section::setSectionName(const char *inSectionName)
{
	strcpy_s(sectionName, sizeof(sectionName), inSectionName);
}

void Section::addPadding()
{
	auto paddingCount = sectionDataBuffer.size() % alignment;

	if(paddingCount != 0)
	{
		paddingCount = alignment - paddingCount;

		for (uint32_t i = 0; i < paddingCount; ++i)
		{
			sectionDataBuffer.push_back(0x90); // add some NOPs
		}
	}
}