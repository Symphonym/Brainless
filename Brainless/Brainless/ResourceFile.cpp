#include "ResourceFile.h"
#include "ResourceLoader.h"
#include "Utility.h"
#include <fstream>

ResourceFile::ResourceFile()
{

}

bool ResourceFile::loadResourceFile(const std::string &fileName, bool unloadAll)
{
	std::ifstream reader(fileName);

	bool hasLoaded = false;
	if (reader.is_open())
	{
		std::string line;
		while (std::getline(reader, line))
		{
			// Skip empty lines
			if (line.empty())
				continue;
			else if (line[0] == '#')
				continue;

			std::vector<std::string> stringData = Utility::splitString(line, ',');
			std::string resourceType = stringData[0].substr(1, stringData[0].size());


			// Add a new resource
			if (stringData[0][0] == '+')
			{

				if (resourceType == "Texture")
				{
					if (unloadAll)
						ResourceLoader::instance().unloadTexture(stringData[2]);
					else
						ResourceLoader::instance().loadTexture(stringData[2], stringData[1]);
				}
				else if (resourceType == "Font")
				{
					if (unloadAll)
						ResourceLoader::instance().unloadFont(stringData[2]);
					else
						ResourceLoader::instance().loadFont(stringData[2], stringData[1]);
				}
				else if (resourceType == "Sound")
				{
					if (unloadAll)
						ResourceLoader::instance().unloadSound(stringData[2]);
					else
						ResourceLoader::instance().loadSound(stringData[2], stringData[1]);
				}
				else if (resourceType == "Music")
				{
					if (unloadAll)
						ResourceLoader::instance().unloadMusic(stringData[2]);
					else
						ResourceLoader::instance().loadMusic(stringData[2], stringData[1]);
				}
				else if (resourceType == "Shader")
				{
					if (unloadAll)
						ResourceLoader::instance().unloadShader(stringData[2]);
					else
						ResourceLoader::instance().loadShader(stringData[2], stringData[1]);
				}
				else if (resourceType == "ResourceFile")
				{
					if (unloadAll)
						loadResourceFile("loadfiles/" + stringData[1], true);
					else
						loadResourceFile("loadfiles/" + stringData[1], false);
				}
				else
					continue;

			}

			// Remove existing resource
			else if (stringData[0][0] == '-')
			{

				if (resourceType == "Texture")
					ResourceLoader::instance().unloadTexture(stringData[1]);
				else if (resourceType == "Font")
					ResourceLoader::instance().unloadFont(stringData[1]);
				else if (resourceType == "Sound")
					ResourceLoader::instance().unloadSound(stringData[1]);
				else if (resourceType == "Music")
					ResourceLoader::instance().unloadMusic(stringData[1]);
				else if (resourceType == "Shader")
					ResourceLoader::instance().unloadShader(stringData[1]);
				else if (resourceType == "ResourceFile")
					loadResourceFile("loadfiles/"+stringData[1], true);
				else
					continue;
			}
		}

		hasLoaded = true;
	}
	reader.close();
	return hasLoaded;
}

int ResourceFile::countResourceCalls(const std::string &fileName)
{
	std::ifstream reader(fileName);

	int callCount = 0;
	if (reader.is_open())
	{
		std::string line;
		while (std::getline(reader, line))
		{
			// Skip empty lines
			if (line.empty())
				continue;
			else if (line[0] == '#')
				continue;

			std::vector<std::string> stringData = Utility::splitString(line, ',');
			std::string resourceType = stringData[0].substr(1, stringData[0].size());


			// Count resource calls
			if (stringData[0][0] == '+' || stringData[0][0] == '-')
			{
				if (resourceType == "Texture")
					++callCount;
				else if (resourceType == "Font")
					++callCount;
				else if (resourceType == "Sound")
					++callCount;
				else if (resourceType == "Music")
					++callCount;
				else if (resourceType == "Shader")
					++callCount;

				// Recurse into other files
				else if (resourceType == "ResourceFile")
					callCount += countResourceCalls("loadfiles/" + stringData[1]);
				else
					continue;

			}
		}
	}
	reader.close();

	return callCount;
}
