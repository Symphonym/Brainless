#ifndef INCLUDED_UTILITY_H
#define INCLUDED_UTILITY_H

#include <vector>
#include <string>
#include <sstream>

class Utility
{
public:

	template<typename TType> static TType clampValue(TType value, TType min, TType max)
	{
		if (value < min)
			return min;
		else if (value > max)
			return max;
		else
			return value;
	};

	static std::vector<std::string> splitString(const std::string &stringValue, char delimeter);

	template<typename TNumberType> static TNumberType stringToNumber(const std::string &stringValue)
	{
		std::stringstream ss;
		ss << stringValue;

		TNumberType number;
		ss >> number;
		return number;
	};

	static float randomValueBetween(float min, float max);
};

#endif