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

void TileMap::draw(const sf::View &view)
{
	sf::Vector2f topLeft(
		view.getCenter().x - view.getSize().x / 2,
		view.getCenter().y - view.getSize().y / 2);

	sf::Vector2f bottomRight(
		view.getCenter().x + view.getSize().x / 2,
		view.getCenter().y + view.getSize().y / 2);

	sf::Vector2i startIndex = positionToIndex(bottomRight);
	sf::Vector2i endIndex = positionToIndex(topLeft);

	for (int x = startIndex.x; x > endIndex.x; x--)
	{
		for (int y = startIndex.y; y > endIndex.x; y--)
		{
			m_tileMap[x][y].draw();
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