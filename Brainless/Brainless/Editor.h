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

	// Which block you currently have selected for drawing
	int m_currentBlock;

	// Which sync ID you are using when placing an item, used to connect items
	// to eachother. A key and the door it goes to would have a matching syncID
	int m_currentSyncID;

	// Update loop
	void loop();

	// Rendering function
	void draw();

	sf::RenderWindow m_editor;
};

#endif