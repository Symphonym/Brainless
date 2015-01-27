#ifndef INCLUDED_EDITOR_GRID_MODE
#define INCLUDED_EDITOR_GRID_MODE

#include "TileMap.h"
#include "Tile.h"
#include <SFML\Graphics.hpp>

class EditorGridMode
{
public:

	EditorGridMode(TileMap &tilemap);
	~EditorGridMode();


	// Returns true if something was changed in the tilemap
	bool events(const sf::Event &event, const sf::RenderWindow &editorWindow);
	bool update(float deltaTime, const sf::RenderWindow &editorWindow);
	void draw();

private:

	// The tilemap to edit
	TileMap &m_tilemap;

	Tile m_currentTile; // Selected tile for grid placement

	// Texture is saved here since it is created as a blank texture from memory
	// ,not loaded as a file
	sf::Sprite m_highlightSprite;
	sf::Texture m_highlightTexture;

};

#endif