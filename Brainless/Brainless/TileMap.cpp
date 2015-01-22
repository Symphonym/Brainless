#include "TileMap.h"

TileMap::TileMap(TileMapLayout layout, const unsigned int tileSize)
:
m_tileSize(tileSize)
{
	// Iterate through map layout and create tiles from the corresponding tile type
	for (std::size_t x = 0; x < layout.size(); x++)
	{
		m_tileMap.push_back(TileVector());
		for (std::size_t y = 0; y < layout[y].size(); y++)
		{
			Tile tile(sf::FloatRect(x*tileSize, y*tileSize, tileSize, tileSize), layout[x][y], sf::Vector2f(0,0));
			m_tileMap[y].push_back(tile);
		}
			
	}
}

// Converts a position to an index in the grid
sf::Vector2i TileMap::positionToIndex(const sf::Vector2f &position)
{
	return sf::Vector2i(position.x / m_tileSize, position.y / m_tileSize);
}

Tile &TileMap::getTile(int x, int y)
{
	return m_tileMap.at(x).at(y);
}