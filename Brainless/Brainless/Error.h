#ifndef INCLUDED_ERROR_H
#define INCLUDED_ERROR_H

#include <string>

#define BRAINLESS_ERROR(x) Error::throwError(x, __FILE__, __LINE__)

class Error
{
public:

	static void throwError(const std::string &errorMsg, const std::string &fileName, int lineNumber);
};

#endif