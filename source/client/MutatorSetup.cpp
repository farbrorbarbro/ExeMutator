#include "MutatorSetup.h"
#include <fstream>
#include "mutator/Mutator.h"
#include "mutator/Random/LinearCongruentialGenerator.h"
#include "mutator/Random/MersenneTwisterGenerator.h"
#include "mutator/Random/RandomDeviceGenerator.h"
#include "mutator/Schemes/GarbageInsertScheme.h"
#include "mutator/Schemes/ScrambleScheme.h"
#include "mutator/Schemes/SubstitutionScheme.h"
#include "ClientException.h"
#include "mutator/Mutator.h"
#include "exeedit/ExeEdit.h"
#include "mutator/CodeAnalysHelper.h"
#include <string>


void MutatorSetup::parseSettings(const char *jsonFilePath)
{
	// read json
	std::ifstream inFile(jsonFilePath);

	if (!inFile.is_open())
	{
		throw ClientException("Failed to open file!");
	}

	nlohmann::json parsedJSON = nlohmann::json::parse(inFile);

	inFile.close();

	uint32_t randCount				= parsedJSON.count("rand");
	uint32_t schemesSectionCount	= parsedJSON.count("schemes");
	uint32_t functionsSectionsCount	= parsedJSON.count("functions");

	if (	randCount != 1 
		||	schemesSectionCount != 1 
		||	functionsSectionsCount != 1)
	{
		throw ClientException("Incorrect input structure!");
	}

	readRandomGeneratorID(parsedJSON);
	readSchemes(parsedJSON);
	readFunctionOffsets(parsedJSON);
}

void MutatorSetup::setupMutator(class Mutator &mutator) const
{
	mutator.setRandomGenerator(randomGenerator.get());

	for (auto &mutationScheme : mutationSchemes)
	{
		mutator.addScheme(mutationScheme.get());
	}
}

void MutatorSetup::runMutation(class Mutator &mutator, class ExeEdit &exeEdit) const
{
	// #todo: fix hard coded section size
	auto &newSection = exeEdit.createNewSection(".text2", 0x1000);

	for (uint32_t originalFunctionOffset : functionOffsets)
	{
		// parse original function
		auto asmFunction = exeEdit.parseFunctionAt(".text", originalFunctionOffset);

		auto testString = asmFunction.toString();
		std::cout << testString << std::endl;


		// mutate
		auto nextVirtualAddress = newSection.getNextVirtualAdress();
		mutator.applySchemes(asmFunction.getData(), asmFunction.getVirtualAdress(), nextVirtualAddress);

		std::cout << CodeAnalysHelper::getString(asmFunction.getData(), asmFunction.getVirtualAdress()) << std::endl;


		// add to new section and register override
		auto newFunctionOffset = newSection.addFunction(asmFunction);

		exeEdit.overrideFunctionAt(
			newSection
			, newFunctionOffset
			, asmFunction
		);
	}
}

void MutatorSetup::readRandomGeneratorID(nlohmann::json &parsedJSON)
{
	int32_t randID = parsedJSON["rand"];

	if (randID == 0)
	{
		randomGenerator.reset(new LinearCongruentialGenerator());
	}
	else if (randID == 1)
	{
		randomGenerator.reset(new MersenneTwisterGenerator());
	}
	else if (randID == 2)
	{
		randomGenerator.reset(new RandomDeviceGenerator());
	}
	else
	{
		throw ClientException("incorrect random generator index!");
	}
}

void MutatorSetup::readSchemes(nlohmann::json &parsedJSON)
{
	nlohmann::json::reference schemesArray = parsedJSON["schemes"];
	
	if (!schemesArray.is_array())
	{
		throw ClientException("Schemes should be an array!");
	}

	for (nlohmann::json::iterator it = schemesArray.begin(); it != schemesArray.end(); ++it) 
	{
		auto &schemeObject = *it;

		uint32_t schemeID	= schemeObject["ID"];

		if (schemeID == 0)
		{
			readGarbageScheme(schemeObject);
		}
		else if (schemeID == 1)
		{
			readScrambleScheme(schemeObject);
		}
		else if (schemeID == 2)
		{
			readSubstitutionScheme(schemeObject);
		}
		else
		{
			throw ClientException("Incorrect random generator index!");
		}
	}
}

void MutatorSetup::readGarbageScheme(nlohmann::json &schemeJSON)
{
	auto garbageInsertScheme = std::make_unique<GarbageInsertScheme>();

	uint32_t min = schemeJSON["min"];
	uint32_t max = schemeJSON["max"];

	garbageInsertScheme->setMinBlock(min);
	garbageInsertScheme->setMaxBlock(max);

	mutationSchemes.push_back(std::move(garbageInsertScheme));
}

void MutatorSetup::readScrambleScheme(nlohmann::json &schemeJSON)
{
	auto scrambleScheme = std::make_unique<ScrambleScheme>();

	uint32_t min = schemeJSON["min"];
	uint32_t max = schemeJSON["max"];

	scrambleScheme->setMinBlock(min);
	scrambleScheme->setMaxBlock(max);

	mutationSchemes.push_back(std::move(scrambleScheme));
}

void MutatorSetup::readSubstitutionScheme(nlohmann::json &schemeJSON)
{
	auto substitutionScheme = std::make_unique<SubstitutionScheme>();

	mutationSchemes.push_back(std::move(substitutionScheme));
}

void MutatorSetup::readFunctionOffsets(nlohmann::json &parsedJSON)
{
	nlohmann::json::reference schemesArray = parsedJSON["functions"];

	if (!schemesArray.is_array())
	{
		throw ClientException("Schemes should be an array!");
	}

	for (nlohmann::json::iterator it = schemesArray.begin(); it != schemesArray.end(); ++it) 
	{
		auto &schemeObject = *it;

		if (!schemeObject.is_string())
		{
			throw ClientException("Expected function offset!");
		}

		std::string hexString = schemeObject;

		uint32_t functionOffset = parseHexString(hexString);

		functionOffsets.push_back(functionOffset);
	}
}

uint32_t MutatorSetup::parseHexString(const std::string &hexString)
{
	uint32_t result	= 0;
	uint32_t base	= 1;

	for (int32_t i = hexString.size() - 1; i >= 0; --i)
	{
		char currentChar = hexString[i];
		uint32_t charValue = 0;


		switch (currentChar)
		{
		case '0':
			charValue = 0x0;
			break;

		case '1':
			charValue = 0x1;
			break;

		case '2':
			charValue = 0x2;
			break;

		case '3':
			charValue = 0x3;
			break;

		case '4':
			charValue = 0x4;
			break;

		case '5':
			charValue = 0x5;
			break;

		case '6':
			charValue = 0x6;
			break;

		case '7':
			charValue = 0x7;
			break;

		case '8':
			charValue = 0x8;
			break;

		case '9':
			charValue = 0x9;
			break;

		case 'a':
		case 'A':
			charValue = 0xA;
			break;

		case 'b':
		case 'B':
			charValue = 0xB;
			break;

		case 'c':
		case 'C':
			charValue = 0xC;
			break;

		case 'd':
		case 'D':
			charValue = 0xD;
			break;

		case 'e':
		case 'E':
			charValue = 0xE;
			break;

		case 'f':
		case 'F':
			charValue = 0xF;
			break;

		default:
			throw ClientException("Failed to parse hex string!");
			break;
		}


		result += charValue * base;

		base *= 16;
	}

	return result;
}