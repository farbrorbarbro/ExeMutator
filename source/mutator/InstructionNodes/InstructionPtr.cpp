#include "InstructionPtr.h"
#include "Instruction.h"


void InstructionDeleter::operator()(class Instruction *instruction) const
{
	delete instruction;
}