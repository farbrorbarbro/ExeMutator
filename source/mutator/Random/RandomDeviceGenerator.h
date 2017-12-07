#pragma once

#include "RandomGenerator.h"
#include <random>


class RandomDeviceGenerator : public RandomGenerator
{
public:

	RandomDeviceGenerator() {}
	
	uint32_t generateNumber() override;

private:

	std::random_device generator;
};