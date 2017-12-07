#pragma once

#include <memory>


struct InstructionDeleter
{
	void operator()(class Instruction *instruction) const;
};


typedef std::unique_ptr<class Instruction, InstructionDeleter> InstructionPtr;