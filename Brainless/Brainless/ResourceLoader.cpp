#include "ResourceLoader.h"
#include <stdexcept>
#include "Error.h"
#include "ResourceFile.h"

ResourceLoader::ResourceLoader()
{

}

ResourceLoader& ResourceLoader::instance()
{
	static ResourceLoader theInstance;
	return theInstance;
}

void ResourceLoader::setLoadingHandler(LoadingHandler handler)
{
	m_handler = handler;
}

void ResourceLoader::loadTexture(const std::string &name, const std::string &filePath)
{
	auto itr = m_textures.find(name);

	// A texture by this name does not exist already
	if (itr == m_textures.end())
	{
		if (m_handler)
			m_handler("Loading texture (" + name + "): " + filePath);

		TexturePtr texPtr = TexturePtr(new sf::Texture());
		if (texPtr->loadFromFile(filePath))
			m_textures[name] = std::move(texPtr);
		else
			BRAINLESS_ERROR("The file '" + filePath + "' could not be loaded!");
	}
}
void ResourceLoader::loadShader(const std::string &name, const std::string &filePath)
{
	if (!sf::Shader::isAvailable())
	{
		BRAINLESS_ERROR("This system does not support shaders, you can't play this game!");
		return;
	}

	auto itr = m_shaders.find(name);

	// A shader by this name does not exist already
	if (itr == m_shaders.end())
	{
		if (m_handler)
			m_handler("Loading shader (" + name + "): " + filePath);

		// TODO, only supports fragment shaders for now
		ShaderPtr shadPtr = ShaderPtr(new sf::Shader());
		if (shadPtr->loadFromFile(filePath, sf::Shader::Fragment))
			m_shaders[name] = std::move(shadPtr);
		else
			BRAINLESS_ERROR("The file '" + filePath + "' could not be loaded!");
	}
}
void ResourceLoader::loadSound(const std::string &name, const std::string &filePath)
{
	auto itr = m_sounds.find(name);

	// A sound by this name does not exist already
	if (itr == m_sounds.end())
	{
		if (m_handler)
			m_handler("Loading sound (" + name + "): " + filePath);

		SoundPtr soundPtr = SoundPtr(new sf::SoundBuffer());
		if (soundPtr->loadFromFile(filePath))
			m_sounds[name] = std::move(soundPtr);
		else
			BRAINLESS_ERROR("The file '" + filePath + "' could not be loaded!");
	}
}
void ResourceLoader::loadMusic(const std::string &name, const std::string &filePath)
{
	auto itr = m_music.find(name);

	// A music file by this name does not exist already
	if (itr == m_music.end())
	{
		if (m_handler)
			m_handler("Opening music file (" + name + "): " + filePath);

		MusicPtr musicPtr = MusicPtr(new sf::Music());
		if (musicPtr->openFromFile(filePath))
			m_music[name] = std::move(musicPtr);
		else
			BRAINLESS_ERROR("The file '" + filePath + "' could not be loaded!");
	}
}
void ResourceLoader::loadFont(const std::string &name, const std::string &filePath)
{
	auto itr = m_fonts.find(name);

	// A font by this name does not exist already
	if (itr == m_fonts.end())
	{
		if (m_handler)
			m_handler("Loading font (" + name + "): " + filePath);

		FontPtr fontPtr = FontPtr(new sf::Font());
		if (fontPtr->loadFromFile(filePath))
			m_fonts[name] = std::move(fontPtr);
		else
			BRAINLESS_ERROR("The file '" + filePath + "' could not be loaded!");
	}
}

sf::Texture& ResourceLoader::retrieveTexture(const std::string &name)
{
	auto itr = m_textures.find(name);

	// A texture by this name does not exist already
	if (itr == m_textures.end())
		BRAINLESS_ERROR("The texture resource '" + name + "' could not be found!");
	else
		return *itr->second.get();
}
sf::Shader& ResourceLoader::retrieveShader(const std::string &name)
{
	auto itr = m_shaders.find(name);

	// A shader by this name does not exist already
	if (itr == m_shaders.end())
		BRAINLESS_ERROR("The shader resource '" + name + "' could not be found!");
	else
		return *itr->second.get();
}
sf::SoundBuffer& ResourceLoader::retrieveSound(const std::string &name)
{
	auto itr = m_sounds.find(name);

	// A sound by this name does not exist already
	if (itr == m_sounds.end())
		BRAINLESS_ERROR("The sound resource '" + name + "' could not be found!");
	else
		return *itr->second.get();
}
sf::Music& ResourceLoader::retrieveMusic(const std::string &name)
{
	auto itr = m_music.find(name);

	// A sound by this name does not exist already
	if (itr == m_music.end())
		BRAINLESS_ERROR("The music resource '" + name + "' could not be found!");
	else
		return *itr->second.get();
}
sf::Font& ResourceLoader::retrieveFont(const std::string &name)
{
	auto itr = m_fonts.find(name);

	// A sound by this name does not exist already
	if (itr == m_fonts.end())
		BRAINLESS_ERROR("The font resource '" + name + "' could not be found!");
	else
		return *itr->second.get();
}

bool ResourceLoader::unloadTexture(const std::string &name)
{
	auto itr = m_textures.find(name);
	if (itr != m_textures.end()) // Resource was found
	{
		if (m_handler)
			m_handler("Unloaded texture: " + name);
		m_textures.erase(itr);
		return true;
	}
	else
		return false;
		
}
bool ResourceLoader::unloadShader(const std::string &name)
{
	auto itr = m_shaders.find(name);
	if (itr != m_shaders.end()) // Resource was found
	{
		if (m_handler)
			m_handler("Unloaded shader: " + name);
		m_shaders.erase(itr);
		return true;
	}
	else
		return false;
}
bool ResourceLoader::unloadSound(const std::string &name)
{
	auto itr = m_sounds.find(name);
	if (itr != m_sounds.end()) // Resource was found
	{
		if (m_handler)
			m_handler("Unloaded sound: " + name);
		m_sounds.erase(itr);
		return true;
	}
	else
		return false;
}
bool ResourceLoader::unloadMusic(const std::string &name)
{
	auto itr = m_music.find(name);
	if (itr != m_music.end()) // Resource was found
	{
		if (m_handler)
			m_handler("Unloaded music: " + name);
		itr->second->stop();
		m_music.erase(itr);
		return true;
	}
	else
		return false;
}
bool ResourceLoader::unloadFont(const std::string &name)
{
	auto itr = m_fonts.find(name);
	if (itr != m_fonts.end()) // Resource was found
	{
		if (m_handler)
			m_handler("Unloaded font: " + name);
		m_fonts.erase(itr);
		return true;
	}
	else
		return false;
}


bool ResourceLoader::loadFromFile(const std::string &fileName)
{
	ResourceFile file(fileName);
	return file.hasLoaded();
}