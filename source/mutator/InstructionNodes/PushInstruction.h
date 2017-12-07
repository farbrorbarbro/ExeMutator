#pragma once

#include "Instruction.h"
#include "RegisterDefinitions.h"


class PushInstruction : public Instruction
{
public:

	PushInstruction(RegisterDefinitions inRegister);
};