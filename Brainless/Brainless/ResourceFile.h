#ifndef INCLUDED_RESOURCE_FILE_H
#define INCLUDED_RESOURCE_FILE_H

#include <string>

class ResourceFile
{
public:

	ResourceFile();

	bool loadResourceFile(const std::string &fileName, bool unloadAll = false);

	// Counts the amount of load/unloads in a resource file, excluding ResourceFile load/unload calls
	int countResourceCalls(const std::string &fileName);

};

#endif