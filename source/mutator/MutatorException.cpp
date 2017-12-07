#include "MutatorException.h"


MutatorException::MutatorException(const char *inErrorMessage)
	: errorMessage{ inErrorMessage }
{
}

char const* MutatorException::what() const
{
	return errorMessage;
}