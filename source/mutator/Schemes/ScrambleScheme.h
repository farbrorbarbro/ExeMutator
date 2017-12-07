#pragma once

#include "MutationScheme.h"


class ScrambleScheme : public MutationScheme
{
public:

	ScrambleScheme() {}
	
	void applyScheme(
		std::vector<uint8_t>	&inData,
		uint32_t				virtualAddress,
		uint32_t				newVirtualAddress
	) override;

	uint32_t	getMinBlock() const;
	void		setMinBlock(uint32_t val);
	
	uint32_t	getMaxBlock() const;
	void		setMaxBlock(uint32_t val);

private:

	uint32_t	generateBlockSize(uint32_t max);

private:

	uint32_t minBlock = 1;
	uint32_t maxBlock = 5;
};

