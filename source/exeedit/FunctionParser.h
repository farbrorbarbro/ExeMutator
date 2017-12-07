#pragma once

#include <vector>
#include <string>
#include "distorm/distorm.h"


class FunctionParser
{
public:

	FunctionParser(const uint8_t *inData, uint32_t inDataMaxLenght);

	bool tryParse();

	std::vector<_DInst>		getDecodedInstructions() const;
	std::vector<uint8_t>	getDataBuffer() const;

	std::string				toString() const;

	uint32_t getVirtualAdress() const { return virtualAdress; }
	void setVirtualAdress(uint32_t val) { virtualAdress = val; }

private:

	void forceParsingUnitl(uint32_t inMinParsingLength);
	bool isForcedParsing() const;

	bool parseNextInstruction(_DInst &decodedInstruction);

private:

	const uint8_t					*data;
	uint32_t						dataMaxLenght;
	uint32_t						dataOffset;
	uint32_t						virtualAdress;

	_CodeInfo						codeInfo;
	uint32_t						minParsingLength;

	std::vector<_DInst>				decodedInstructions;
	std::vector<uint8_t>			dataBuffer;
};
