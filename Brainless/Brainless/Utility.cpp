#include "Utility.h"
#include <sstream>

std::vector<std::string> Utility::splitString(const std::string &stringValue, char delimeter)
{
	std::vector<std::string> result;
	std::stringstream splitter(stringValue);

	std::string resultString;
	while (std::getline(splitter, resultString, delimeter))
		result.push_back(resultString);

	return result;
}