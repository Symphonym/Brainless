#ifndef INCLUDED_EDITOR_H
#define INCLUDED_EDITOR_H

#include <SFML\Graphics.hpp>
#include <memory>
#include "Tile.h"

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
	sf::View m_camera, m_hudCamera;

	

	// Texture is saved here since it is created as a blank texture from memory
	// ,not loaded as a file
	sf::Sprite m_highlightSprite;
	sf::Texture m_highlightTexture;

	// Whether or not you have saved
	sf::Text m_saveText;


	// HUD data

	// Which block you currently have selected for drawing
	Tile m_currentTile;

	// Which sync ID you are using when placing an item, used to connect items
	// to eachother. A key and the door it goes to would have a matching syncID
	int m_currentSyncID;



	// Load/save functionality
	void loadFile();
	void saveFile();

	// Update loop
	void loop();

	// Rendering function
	void draw();

	sf::RenderWindow m_editor;
};

#endif