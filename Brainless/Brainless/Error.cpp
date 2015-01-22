#include "Error.h"
#include <sstream>
#include <iostream>

void Error::throwError(const std::string &errorMsg, const std::string &fileName, int lineNumber)
{
	std::stringstream ss;
	ss << lineNumber;

	std::cerr << "FATAL ERROR (" << fileName << ":" << lineNumber << "): " << errorMsg << std::endl;
	throw std::runtime_error("FATAL ERROR(" + fileName + ":" + ss.str() + ") : " + errorMsg);
}