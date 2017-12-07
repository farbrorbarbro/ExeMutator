#include "PushInstruction.h"


PushInstruction::PushInstruction(RegisterDefinitions inRegister)
{
	instructionBytes[0]	= 0x50 | (uint8_t)inRegister;
	instructionLength	= 1;
}