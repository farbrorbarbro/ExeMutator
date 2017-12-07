#pragma once

#include "RandomGenerator.h"
#include <random>


class LinearCongruentialGenerator : public RandomGenerator
{
public:

	LinearCongruentialGenerator();

	void		initiate(uint32_t initialSeed);
	uint32_t	generateNumber() override;

private:

	std::minstd_rand generator;
};