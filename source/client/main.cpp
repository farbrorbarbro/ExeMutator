#include <stdio.h>
#include "distorm/distorm.h"
#include "distorm/mnemonics.h"
#include "asmtk/asmtk.h"
#include <string>
#include <iostream>
#include "exeedit/ExeEdit.h"
#include "Mutator.h"
#include "Random/MersenneTwisterGenerator.h"
#include "Schemes/ScrambleScheme.h"
#include "Schemes/GarbageInsertScheme.h"
#include "Schemes/SubstitutionScheme.h"
#include "mutator/CodeAnalysHelper.h"
#include "MutatorSetup.h"
#include "ClientException.h"
#include "exeedit/ExeEditException.h"
#include "mutator/MutatorException.h"


/*
==========================================================================================
// usage:

program <jsonFile> binaryToMutate

==========================================================================================


==========================================================================================
// example of json file
{
	// specify which random generator to use
	// 0 = LinearCongruentialGenerator
	// 1 = MersenneTwisterGenerator
	// 2 = RandomDeviceGenerator

	"rand" : 1, 

	// specify which mutation schemes to use (many can be used, in any order)
	// 0 = garbage insertion 
		min and max parameters specifies how much garbage that is generated per block
	// 1 = scramble with unconditional jumps
		min and max parameters specifies block size between jumps
	// 2 = substitute supported instructions

	"schemes": 
	[
		{
			"ID": 0,
			"min": 1,
			"max": 3
		},
		{
			"ID": 2
		},
		{
			"ID": 1,
			"min": 1,
			"max": 5
		}
	],

	// specify original functions' offset in code section, all will be mutated
	"functions": 
	[
		"510"
	]
}
==========================================================================================
*/


int main(int argc, char *args[])
{
	if (argc < 3)
	{
		std::cout	<< "No parameter was passed!" << std::endl
					<< "Usage:" << std::endl
					<< "mutator <settings.json> <program>:" << std::endl;

		std::cin.clear();
		std::cin.get();
		return -1;
	}

	std::string	jsonFile	= args[1];
	std::string programName	= args[2];


	// parse mutation settings
	MutatorSetup mutatorSetup;

	try
	{
		mutatorSetup.parseSettings(jsonFile.c_str());
	}
	catch (ClientException &ex)
	{
		std::cout	<< "Failed to apply settings:" << std::endl 
					<< ex.what() << std::endl;

		std::cin.clear();
		std::cin.get();
		return -1;
	}
	catch (...)
	{
		std::cout << "Unexpected JSON structure!" << std::endl;

		std::cin.clear();
		std::cin.get();
		return -1;
	}


	// load original binary into exe edit instance
	ExeEdit exeEdit;

	try
	{
		exeEdit.loadBinaryFile(programName.c_str());
	}
	catch (ExeEditException &ex)
	{
		std::cout	<< "Failed to read binary file!" << std::endl
					<< ex.what() << std::endl;

		std::cin.clear();
		std::cin.get();
		return -1;
	}
	

	try
	{
		// run mutation
		Mutator mutator;

		mutatorSetup.setupMutator(mutator);

		mutatorSetup.runMutation(mutator, exeEdit);
	}
	catch(MutatorException &ex)
	{
		std::cout << "Failed to run mutation!" << std::endl
			<< ex.what() << std::endl;

		std::cin.clear();
		std::cin.get();
		return -1;
	}

	// write changes to new binary file
	std::string newBinaryName = programName ;
	newBinaryName.erase(newBinaryName.end() - 4, newBinaryName.end());
	newBinaryName += "_modified.exe";

	exeEdit.writeBinaryFile(newBinaryName.c_str());

	return 0;
}