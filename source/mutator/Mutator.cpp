#include "Mutator.h"
#include "MutationScheme.h"


bool Mutator::applySchemes(
	std::vector<uint8_t>	&inData,
	uint32_t				virtualAddress,
	uint32_t				newVirtualAddress
)
{
	for (uint32_t i = 0; i < mutationSchemes.size(); ++i)
	{
		auto *currentScheme = mutationSchemes[i];

		currentScheme->applyScheme(inData, virtualAddress, newVirtualAddress);

		// update virtual address
		virtualAddress = newVirtualAddress;
	}

	return true;
}

void Mutator::addScheme(class MutationScheme *newScheme)
{
	mutationSchemes.push_back(newScheme);
	newScheme->injectRandomGenerator(randomGenerator);
}

void Mutator::clearAllSchemes()
{
	injectRandomGenerator(nullptr);
	mutationSchemes.clear();
}

void Mutator::reset()
{
	clearAllSchemes();

	randomGenerator = nullptr;
}

void Mutator::setRandomGenerator(class RandomGenerator *inRandomGenerator)
{
	randomGenerator = inRandomGenerator;

	injectRandomGenerator(inRandomGenerator);
}

void Mutator::injectRandomGenerator(class RandomGenerator * inRandomGenerator)
{
	for (auto *scheme : mutationSchemes)
	{
		scheme->injectRandomGenerator(inRandomGenerator);
	}
}
