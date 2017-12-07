#pragma once

#include <vector>


// holds an assembler functions bytes
class AsmFunction
{
public:

	AsmFunction() {}

	bool tryParse(uint8_t *inData, uint32_t inDataMaxLenght, uint32_t inVirtualAdress);

	bool isValid() const;

	uint32_t					getOriginalSize() const;
	uint32_t					getFunctionLength() const;
	const std::vector<uint8_t>&	getData() const;
	std::vector<uint8_t>&		getData();

	std::string					toString() const;

	const char*					getSectionName() const;
	void						setSectionName(const char *val);
	
	uint32_t					getOffset() const;
	void						setOffset(uint32_t val);

	uint32_t					getVirtualAdress() const;

private:

	const char				*sectionName	= nullptr;
	uint32_t				offset			= 0;
	uint32_t				virtualAdress	= 0;

	uint32_t				originalSize	= 0;
	std::vector<uint8_t>	dataBuffer;
};