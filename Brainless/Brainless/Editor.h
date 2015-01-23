#ifndef INCLUDED_EDITOR_H
#define INCLUDED_EDITOR_H

#include <SFML\Graphics.hpp>
#include <memory>

class TileMap;
class Editor
{
public:

	Editor();

	// Starts the engine
	void run();

private:

	typedef std::unique_ptr<TileMap> MapPtr;

	MapPtr m_map;
	sf::View m_camera; 

	// Texture is saved here since it is created as a blank texture from memory
	// ,not loaded as a file
	sf::Sprite m_highlightSprite;
	sf::Texture m_highlightTexture;

	// Update loop
	void loop();

	void draw();

	sf::RenderWindow m_editor;
};

#endif