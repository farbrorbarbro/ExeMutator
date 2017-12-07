#pragma once

#include <exception>


class ExeEditException : public std::exception
{
public:

	ExeEditException(const char *inErrorMessage);
	
	char const* what() const override;

private:

	const char *errorMessage;
};