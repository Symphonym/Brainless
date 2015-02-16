#ifndef INCLUDED_EDITOR_GRID_MODE
#define INCLUDED_EDITOR_GRID_MODE

#include "TileMap.h"
#include "Tile.h"
#include <SFML\Graphics.hpp>
#include <array>
#include <unordered_map>

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

	typedef std::array<Tile::TileTypes, 256> AutotilingData;
	typedef std::pair<int, Tile::TileTypes> AutotilingValue;

	// The tilemap to edit
	TileMap &m_tilemap;

	// Autotiling ranges mapped to strings
	std::unordered_map<std::string, AutotilingData> m_autotiling;
	bool m_autotilingEnabled;
	sf::Text m_autotilingText;

	void addAutotilingRange(const std::string &name, Tile::TileTypes defaultValue, const std::vector<AutotilingValue> &autotilingRange);
	int calculateAutotilingValue(int x, int y, const std::string &autotilingRangeName);
	void parseAutotilingFile(const std::string &fileName, const std::string &rangeName);
	std::vector<sf::Vector2i> getAdjacentTileIndices(int centerX, int centerY);

	Tile m_currentTile; // Selected tile for grid placement

	// Texture is saved here since it is created as a blank texture from memory
	// ,not loaded as a file
	sf::Sprite m_highlightSprite;
	sf::Texture m_highlightTexture;

	// Text for showing the grid index of the mouse pointer
	sf::Text m_indexText;

};

#endif