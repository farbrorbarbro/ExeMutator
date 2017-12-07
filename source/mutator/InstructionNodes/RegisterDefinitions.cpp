#include "RegisterDefinitions.h"
#include "Random\RandomGenerator.h"


RegisterDefinitions getRandom32bitRegister(class RandomGenerator *randomGenerator)
{
	RegisterDefinitions registers[] =
	{
		RegisterDefinitions::EAX,
		RegisterDefinitions::ECX,
		RegisterDefinitions::EDX,
		RegisterDefinitions::EBX,
		//RegisterDefinitions::ESP,
		//RegisterDefinitions::EBP,
		//RegisterDefinitions::ESI,
		//RegisterDefinitions::EDI
	};

	uint32_t index = randomGenerator->generateNumberRange(sizeof(registers) / sizeof(registers[0]));
	return registers[index];
}

RegisterDefinitions getRandom16bitRegister(class RandomGenerator *randomGenerator)
{
	RegisterDefinitions registers[] =
	{
		RegisterDefinitions::AX,
		RegisterDefinitions::CX,
		RegisterDefinitions::DX,
		RegisterDefinitions::BX,
		//RegisterDefinitions::SP,
		//RegisterDefinitions::BP,
		//RegisterDefinitions::SI,
		//RegisterDefinitions::DI
	};

	uint32_t index = randomGenerator->generateNumberRange(sizeof(registers) / sizeof(registers[0]));
	return registers[index];
}

RegisterDefinitions getRandom8bitRegister(class RandomGenerator *randomGenerator)
{
	RegisterDefinitions registers[] =
	{
		RegisterDefinitions::AH,
		RegisterDefinitions::AL,
		RegisterDefinitions::CH,
		RegisterDefinitions::CL,
		RegisterDefinitions::DH,
		RegisterDefinitions::DL,
		RegisterDefinitions::BH,
		RegisterDefinitions::BL
	};

	uint32_t index = randomGenerator->generateNumberRange(sizeof(registers) / sizeof(registers[0]));
	return registers[index];
}

const char* getAsString(RegisterDefinitions registerValue)
{
	switch (registerValue)
	{
	case RegisterDefinitions::EAX:
		return "EAX";
	case RegisterDefinitions::ECX:
		return "ECX";
	case RegisterDefinitions::EDX:
		return "EDX";
	case RegisterDefinitions::EBX:
		return "EBX";
	case RegisterDefinitions::ESP:
		return "ESP";
	case RegisterDefinitions::EBP:
		return "EBP";
	case RegisterDefinitions::ESI:
		return "ESI";
	case RegisterDefinitions::EDI:
		return "EDI";
	case RegisterDefinitions::AX:
		return "AX";
	case RegisterDefinitions::CX:
		return "CX";
	case RegisterDefinitions::DX:
		return "DX";
	case RegisterDefinitions::BX:
		return "BX";
	case RegisterDefinitions::SP:
		return "SP";
	case RegisterDefinitions::BP:
		return "BP";
	case RegisterDefinitions::SI:
		return "SI";
	case RegisterDefinitions::DI:
		return "DI";
	case RegisterDefinitions::AH:
		return "AH";
	case RegisterDefinitions::AL:
		return "AL";
	case RegisterDefinitions::CH:
		return "CH";
	case RegisterDefinitions::CL:
		return "CL";
	case RegisterDefinitions::DH:
		return "DH";
	case RegisterDefinitions::DL:
		return "DL";
	case RegisterDefinitions::BH:
		return "BH";
	case RegisterDefinitions::BL:
		return "BL";
	default:
		return "Error!";
	}
}
