#include "RandomGenerator.h"


bool RandomGenerator::generateBool()
{
	return (generateNumber() % 2) == 1;
}

uint32_t RandomGenerator::generateNumberRange(uint32_t lessThanNumber)
{
	return generateNumber() % lessThanNumber;
}

bool RandomGenerator::generateRandomPercent(uint32_t percent)
{
	if (percent == 0)
	{
		return false;
	}

	if (percent >= 100)
	{
		return true;
	}

	// [1..100]
	uint32_t randomPercent = generateNumberRange(100) + 1;

	return randomPercent < percent;
}