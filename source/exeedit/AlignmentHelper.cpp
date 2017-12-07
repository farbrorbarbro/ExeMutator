#include "AlignmentHelper.h"


uint32_t AlignmentHelper::align(uint32_t value, uint32_t alignmentValue)
{
	auto rest = value % alignmentValue;

	value += rest != 0
		? alignmentValue - rest
		: 0;

	return value;
}
