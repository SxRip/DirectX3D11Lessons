#pragma once
#include <Windows.h>
#include <string>
#include <exception>
#include <sstream>

class BaseException : public std::exception
{
public:
	BaseException(const char* file, size_t line, HRESULT hr)
		: _file{ file }, _line{ line }, _hr{ hr }
	{}

	BaseException(const BaseException&) = delete;
	BaseException operator=(const BaseException&&) = delete;

	inline virtual const char* get_type() const noexcept { return "Base Exception"; }

protected:
	size_t _line;
	const char* _file;

	HRESULT _hr;
	mutable std::string _error;
};