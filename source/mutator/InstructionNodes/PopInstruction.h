#pragma once

#include "Instruction.h"
#include "RegisterDefinitions.h"


class PopInstruction : public Instruction
{
public:

	PopInstruction(RegisterDefinitions inRegister);
};