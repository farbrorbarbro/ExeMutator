#pragma once

#include "RandomGenerator.h"
#include <random>


class MersenneTwisterGenerator : public RandomGenerator
{
public:

	MersenneTwisterGenerator();

	void		initiate(uint32_t initialSeed);
	uint32_t	generateNumber() override;

private:

	std::mt19937 generator;
};

