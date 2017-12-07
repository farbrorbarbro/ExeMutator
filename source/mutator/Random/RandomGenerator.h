#pragma once

#include <stdint.h>

class RandomGenerator
{
public:
	virtual ~RandomGenerator() {}

	virtual uint32_t	generateNumber() = 0;

	bool				generateBool();
	uint32_t			generateNumberRange(uint32_t lessThanNumber);

	bool				generateRandomPercent(uint32_t percent);
};
