#include "PopInstruction.h"


PopInstruction::PopInstruction(RegisterDefinitions inRegister)
{
	instructionBytes[0]	= 0x58 | (uint8_t)inRegister;
	instructionLength	= 1;
}