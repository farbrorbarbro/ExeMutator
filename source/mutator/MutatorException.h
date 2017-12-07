#pragma once

#include <exception>


class MutatorException : public std::exception
{
public:

	MutatorException(const char *inErrorMessage);

	char const* what() const override;

private:

	const char *errorMessage;
};