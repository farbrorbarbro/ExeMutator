#pragma once

#include <vector>


class Mutator
{
public:

	Mutator() {}
	
	bool applySchemes(
		std::vector<uint8_t>	&inData,
		uint32_t				virtualAddress,
		uint32_t				newVirtualAddress
	);

	void addScheme(class MutationScheme *newScheme);
	void clearAllSchemes();

	void reset();

	void setRandomGenerator(class RandomGenerator *inRandomGenerator);

private:

	void injectRandomGenerator(class RandomGenerator * inRandomGenerator);

private:

	class RandomGenerator					*randomGenerator;

	std::vector<class MutationScheme*>		mutationSchemes;
};