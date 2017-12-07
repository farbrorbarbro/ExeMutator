#pragma once

#include <exception>


class ClientException : public std::exception
{
public:

	ClientException(const char *inErrorMessage);
	
	char const* what() const override;

private:

	const char *errorMessage;
};