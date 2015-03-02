#ifndef INCLUDED_RESOURCE_LOADER_H
#define INCLUDED_RESOURCE_LOADER_H

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>


class ResourceLoader
{
public:

	typedef std::function<void(const std::string &info, int currentlyLoaded, int totalResources)> LoadingHandler;

	void setLoadingHandler(LoadingHandler handler);

	bool musicExists(const std::string &name);


	// Retrieve a resource by name
	sf::Texture& retrieveTexture(const std::string &name);
	sf::Shader& retrieveShader(const std::string &name);
	sf::SoundBuffer& retrieveSound(const std::string &name);
	sf::Music& retrieveMusic(const std::string &name);
	sf::Font& retrieveFont(const std::string &name);


	// Load multiple resources from a resource file
	bool loadResourceFile(const std::string &fileName, bool loadingScreen = true);
	// Unload everything in a resource file
	bool unloadResourceFile(const std::string &fileName, bool loadingScreen = true);


	// Returns the names of all textures currently loaded in memory
	// TODO std::vector<std::string> getTexturesInMemory() const;

	// Singleton access to ResourceLoader
	static ResourceLoader& instance();


private:

	friend class ResourceFile;

	ResourceLoader();

	int m_totalResources; // Total amount of resources to load
	int m_currentResources; // Resources loaded
	bool m_enableLoadingScreen;
	LoadingHandler m_handler;

	typedef std::unique_ptr<sf::Texture> TexturePtr;
	typedef std::unique_ptr<sf::Shader> ShaderPtr;
	typedef std::unique_ptr<sf::SoundBuffer> SoundPtr;
	typedef std::unique_ptr<sf::Music> MusicPtr;
	typedef std::unique_ptr<sf::Font> FontPtr;

	std::unordered_map<std::string, TexturePtr> m_textures;
	std::unordered_map<std::string, ShaderPtr> m_shaders;
	std::unordered_map<std::string, SoundPtr> m_sounds;
	std::unordered_map<std::string, MusicPtr> m_music;
	std::unordered_map<std::string, FontPtr> m_fonts;


	void callHandler(const std::string &info);

	// Loads from resource file, can execute recursively to load other resource files
	bool loadFromFile(const std::string &fileName);

	// Load a resource and map it to the desired name
	void loadTexture(const std::string &name, const std::string &filePath);
	void loadShader(const std::string &name, const std::string &filePath);
	void loadSound(const std::string &name, const std::string &filePath);
	void loadMusic(const std::string &name, const std::string &filePath);
	void loadFont(const std::string &name, const std::string &filePath);

	// Attempt to unload a resource, returns true if the unloading was successfull
	bool unloadTexture(const std::string &name);
	bool unloadShader(const std::string &name);
	bool unloadSound(const std::string &name);
	bool unloadMusic(const std::string &name);
	bool unloadFont(const std::string &name);
};

#endif


// loadTexture("TileTexture");
// 