#pragma once

#include <string>
#include <sstream>

class Converter
{
public:
	template <class T, class U>
	static T convert(const U& source)
	{
		T var;
		std::stringstream ss;

		ss << source;
		ss >> var;

		return var;
	}

	
	template <>
	static std::string convert<std::string, std::string>(const std::string& source)
	{
		return source;
	}


	template <>
	static std::string convert<std::string, const char*>(const char* const& source)
	{
		return source;
	}
};