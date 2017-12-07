#include "MutationScheme.h"
#include "CodeAnalysHelper.h"


void MutationScheme::injectRandomGenerator(class RandomGenerator *inRandomGenerator)
{
	randomGenerator = inRandomGenerator;
}