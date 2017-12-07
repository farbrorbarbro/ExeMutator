#pragma once
#include <memory>
#include <vector>
#include "AsmFunction.h"
#include "FunctionOverrideRecord.h"
#include "Section.h"
#include "HeaderInfo.h"


class ExeEdit
{
public:

	~ExeEdit();

	void loadBinaryFile(const char *filepath);
	void writeBinaryFile(const char *filepath) const;

	void reset();

	AsmFunction parseFunctionAt(
		const char *sectionName
		, uint32_t offset
	);

	void overrideFunctionAt(
		const Section		&newSection
		, uint32_t			newSectionOffset
		, const AsmFunction	&newFunction
	);

	Section& createNewSection(const char *sectionName, uint32_t sectionSize);

private:

	bool readAllBytes(const char *filepath);
	
	static void resolveSectionPointers(
		const uint8_t			*fileStart
		, struct ImageDosHeader	*&out_dosHeader
		, struct ImageNtHeaders	*&out_ntHeaders
		, struct SectionHeader	*&out_sectionHeaders
	);

	void calculateNextVirtualAddress();

private:

	std::unique_ptr<uint8_t> rawFile;
	uint32_t fileLength = 0;

	struct ImageDosHeader		*dosHeader		= nullptr;
	struct ImageNtHeaders		*ntHeader		= nullptr;
	struct SectionHeader		*sectionHeaders	= nullptr;

	HeaderInfo					headerInfo;

	std::vector<Section>				newSections;
	std::vector<FunctionOverrideRecord>	functionOverrideRecords;

	uint32_t					nextVirtualAddress	= 0;
};