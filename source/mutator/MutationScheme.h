#pragma once

#include <vector>
#include <stdint.h>


class MutationScheme
{
public:

	virtual ~MutationScheme() {}
	
	virtual void applyScheme(
		std::vector<uint8_t> &inData, 
		uint32_t virtualAddress, 
		uint32_t newVirtualAddress
	) = 0;
	
	virtual void injectRandomGenerator(class RandomGenerator *inRandomGenerator);

protected:

	class RandomGenerator		*randomGenerator = nullptr;
};