#include "ClientException.h"


ClientException::ClientException(const char *inErrorMessage)
	: errorMessage	{ inErrorMessage }
{
}

char const* ClientException::what() const
{
	return errorMessage;
}