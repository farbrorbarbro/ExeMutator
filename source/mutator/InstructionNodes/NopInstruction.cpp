#include "NopInstruction.h"


NopInstruction::NopInstruction()
{
	instructionBytes[0]	= 0x90;
	instructionLength	= 1;
}