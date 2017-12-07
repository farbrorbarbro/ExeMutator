#pragma once

#include <stdint.h>


enum class RegisterDefinitions : uint8_t
{
	// 32-bit
	EAX = 0,
	ECX,
	EDX,
	EBX,
	ESP,
	EBP,
	ESI,
	EDI,

	// 16-bit
	AX,
	CX,
	DX,
	BX,
	SP,
	BP,
	SI,
	DI,

	// 8-bit
	AH,
	AL,
	CH,
	CL,
	DH,
	DL,
	BH,
	BL
};


RegisterDefinitions getRandom32bitRegister(class RandomGenerator *randomGenerator);
RegisterDefinitions getRandom16bitRegister(class RandomGenerator *randomGenerator);
RegisterDefinitions getRandom8bitRegister(class RandomGenerator *randomGenerator);

const char* getAsString(RegisterDefinitions registerValue);
