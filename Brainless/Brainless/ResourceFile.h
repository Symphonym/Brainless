#ifndef INCLUDED_RESOURCE_FILE_H
#define INCLUDED_RESOURCE_FILE_H

#include <string>

class ResourceFile
{
public:

	ResourceFile(const std::string &fileName);

	// Returns true if the file loaded successfully
	bool hasLoaded() const;

private:

	void loadResourceFile(const std::string &fileName, bool unloadAll = false);

	bool m_hasLoaded;

};

#endif