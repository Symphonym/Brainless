#include "ResourceFile.h"
#include "ResourceLoader.h"
#include "Utility.h"
#include <fstream>

ResourceFile::ResourceFile(const std::string &fileName)
:
m_hasLoaded(false)
{
	loadResourceFile(fileName);
}

bool ResourceFile::hasLoaded() const
{
	return m_hasLoaded;
}

#include <iostream>
void ResourceFile::loadResourceFile(const std::string &fileName)
{
	std::ifstream reader(fileName);

	if (reader.is_open())
	{
		m_hasLoaded = true;

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
				if (stringData.size() != 3)
				{
					m_hasLoaded = false;
					break;
				}

				if (resourceType == "Texture")
				{
					ResourceLoader::instance().loadTexture(stringData[2], stringData[1]);
					std::cout << "Loaded texture: " << stringData[2] << "::::" << stringData[1] << std::endl;
				}
				else if (resourceType == "Font")
					ResourceLoader::instance().loadFont(stringData[2], stringData[1]);
				else if (resourceType == "Sound")
					ResourceLoader::instance().loadSound(stringData[2], stringData[1]);
				else if (resourceType == "Music")
					ResourceLoader::instance().loadMusic(stringData[2], stringData[1]);
				else if (resourceType == "Shader")
					ResourceLoader::instance().loadShader(stringData[2], stringData[1]);
				else
					continue;

			}

			// Remove existing resource
			else if (stringData[0][0] == '-')
			{
				if (stringData.size() != 2)
				{
					m_hasLoaded = false;
					break;
				}

				if (resourceType == "Texture")
				{
					ResourceLoader::instance().unloadTexture(stringData[1]);
					std::cout << "Removed texture: " << stringData[1] << std::endl;
				}
				else if (resourceType == "Font")
					ResourceLoader::instance().unloadFont(stringData[1]);
				else if (resourceType == "Sound")
					ResourceLoader::instance().unloadSound(stringData[1]);
				else if (resourceType == "Music")
					ResourceLoader::instance().unloadMusic(stringData[1]);
				else if (resourceType == "Shader")
					ResourceLoader::instance().unloadShader(stringData[1]);
				else
					continue;
			}
		}
	}
	reader.close();
}
