#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>

#include "Converter.h"

struct Console
{
	struct ConsoleStream
	{
		template <class T>
		ConsoleStream& operator<<(T value)
		{
			::OutputDebugStringA(Converter::convert<std::string>(value).c_str());

			return *this;
		}
	};

	static ConsoleStream out;
	static const char* endl;
};