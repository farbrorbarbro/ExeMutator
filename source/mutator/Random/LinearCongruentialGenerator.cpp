#include "LinearCongruentialGenerator.h"


LinearCongruentialGenerator::LinearCongruentialGenerator()
{
	std::random_device random;
	initiate(random());
}

void LinearCongruentialGenerator::initiate(uint32_t initialSeed)
{
	generator.seed(initialSeed);
}

uint32_t LinearCongruentialGenerator::generateNumber()
{
	return generator();
}
