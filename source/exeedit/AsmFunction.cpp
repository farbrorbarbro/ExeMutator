#include "AsmFunction.h"
#include "FunctionParser.h"


bool AsmFunction::tryParse(
	uint8_t		*inData
	, uint32_t	inDataMaxLenght
	, uint32_t	inVirtualAdress
)
{
	dataBuffer.clear();

	FunctionParser functionParser(inData, inDataMaxLenght);

	functionParser.setVirtualAdress(inVirtualAdress);

	if (!functionParser.tryParse())
	{
		return false;
	}

	// copy data buffer
	dataBuffer		= functionParser.getDataBuffer();
	originalSize	= dataBuffer.size();
	virtualAdress	= inVirtualAdress;

	return true;
}

bool AsmFunction::isValid() const
{
	return dataBuffer.size() > 0;
}

uint32_t AsmFunction::getOriginalSize() const
{
	return originalSize;
}

uint32_t AsmFunction::getFunctionLength() const
{
	return dataBuffer.size();
}

const std::vector<uint8_t>& AsmFunction::getData() const
{
	return dataBuffer;
}

std::vector<uint8_t>& AsmFunction::getData()
{
	return dataBuffer;
}

std::string AsmFunction::toString() const
{
	FunctionParser functionParser(dataBuffer.data(), dataBuffer.size());
	
	if (!functionParser.tryParse())
	{
		return "Failed to parse function!";
	}

	return functionParser.toString();
}

const char* AsmFunction::getSectionName() const
{
	return sectionName;
}

void AsmFunction::setOffset(uint32_t val)
{
	offset = val;
}

uint32_t AsmFunction::getVirtualAdress() const
{
	return virtualAdress;
}

uint32_t AsmFunction::getOffset() const
{
	return offset;
}

void AsmFunction::setSectionName(const char *val)
{
	sectionName = val;
}
