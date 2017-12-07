#pragma once

#include <memory>
#include <vector>
#include "mutator/MutationScheme.h"
#include "mutator/Random/RandomGenerator.h"
#include "json/json.h"


class MutatorSetup
{
public:

	MutatorSetup() {}
	
	void parseSettings(const char *jsonFilePath);
	void setupMutator(class Mutator &mutator) const;
	void runMutation(class Mutator &mutator, class ExeEdit &exeEdit) const;

private:

	void readRandomGeneratorID(nlohmann::json &parsedJSON);
	

	void readSchemes(nlohmann::json &parsedJSON);

	void readGarbageScheme(nlohmann::json &schemeJSON);
	void readScrambleScheme(nlohmann::json &schemeJSON);
	void readSubstitutionScheme(nlohmann::json &schemeJSON);


	void readFunctionOffsets(nlohmann::json &parsedJSON);

	static uint32_t parseHexString(const std::string &hexString);

private:

	std::unique_ptr<RandomGenerator>				randomGenerator;
	std::vector<std::unique_ptr<MutationScheme>>	mutationSchemes;
	std::vector<uint32_t>							functionOffsets;
};