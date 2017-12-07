#pragma once


struct FunctionOverrideRecord
{
	const char		*sectionName;
	uint32_t		offset;
	const char		*newSectionName;
	uint32_t		newSectionOffset;
	AsmFunction		newFunction;
};