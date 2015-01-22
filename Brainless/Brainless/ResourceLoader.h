#ifndef INCLUDED_RESOURCE_LOADER_H
#define INCLUDED_RESOURCE_LOADER_H

#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\Shader.hpp>
#include <SFML\Audio\Sound.hpp>
#include <SFML\Audio\Music.hpp>
#include <memory>
#include <string>
#include <unordered_map>

class ResourceLoader
{
public:

	// Load a resource and map it to the desired name
	void loadTexture(const std::string &name, const std::string &filePath);
	void loadShader(const std::string &name, const std::string &filePath);
	void loadSound(const std::string &name, const std::string &filePath);
	void loadMusic(const std::string &name, const std::string &filePath);

	// Retrieve a resource by name
	sf::Texture& retrieveTexture(const std::string &name);
	sf::Shader& retrieveShader(const std::string &name);
	sf::Sound& retrieveSound(const std::string &name);
	sf::Music& retrieveMusic(const std::string &name);

	// Attempt to unload a resource, returns true if the unloading was successfull
	bool unloadTexture(const std::string &name);
	bool unloadShader(const std::string &name);
	bool unloadSound(const std::string &name);
	bool unloadMusic(const std::string &name);

	// Returns the names of all textures currently loaded in memory
	// TODO std::vector<std::string> getTexturesInMemory() const;

	// Singleton access to ResourceLoader
	static ResourceLoader& instance();


private:

	ResourceLoader();

	typedef std::unique_ptr<sf::Texture> TexturePtr;
	typedef std::unique_ptr<sf::Shader> ShaderPtr;
	typedef std::unique_ptr<sf::Sound> SoundPtr;
	typedef std::unique_ptr<sf::Music> MusicPtr;

	std::unordered_map<std::string, TexturePtr> m_textures;
	std::unordered_map<std::string, ShaderPtr> m_shaders;
	std::unordered_map<std::string, SoundPtr> m_sounds;
	std::unordered_map<std::string, MusicPtr> m_music;
};

#endif


// loadTexture("TileTexture");
// 