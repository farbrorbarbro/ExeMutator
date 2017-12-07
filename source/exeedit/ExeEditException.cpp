#include "ExeEditException.h"


ExeEditException::ExeEditException(const char *inErrorMessage)
	: errorMessage	{ inErrorMessage }
{
}

char const* ExeEditException::what() const
{
	return errorMessage;
}