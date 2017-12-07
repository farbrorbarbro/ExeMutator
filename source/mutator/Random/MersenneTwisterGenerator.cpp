#include "MersenneTwisterGenerator.h"


MersenneTwisterGenerator::MersenneTwisterGenerator()
{
	std::random_device random;
	initiate(random());
}

void MersenneTwisterGenerator::initiate(uint32_t initialSeed)
{
	generator.seed(initialSeed);
}

uint32_t MersenneTwisterGenerator::generateNumber()
{
	return generator();
}
