#include "TileMap.h"
#include "Constants.h"
#include "Utility.h"

TileMap::TileMap(TileMapLayout layout, const unsigned int tileSize)
:
m_tileSize(tileSize)
{
	// Iterate through map layout and create tiles from the corresponding tile type
	for (std::size_t x = 0; x < layout.size(); x++)
	{
		m_tileMap.push_back(TileVector());
		for (std::size_t y = 0; y < layout[x].size(); y++)
		{
			// Tile size has to be the dimension of the front side of the cube
			Tile tile(sf::FloatRect(
				x*tileSize, y*tileSize, tileSize, tileSize),
				layout[x][y],
				// In the vector below
				// X needs to be negative half the width of the left side of the cube
				// Y needs to be negative half the width of the top side of the cube
				sf::Vector2f(Constants::LeftTileOffset, Constants::TopTileOffset));
			m_tileMap[x].push_back(tile);
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

	sf::Vector2i startIndex = positionToIndex(bottomRight) + sf::Vector2i(1, 1);
	sf::Vector2i endIndex = positionToIndex(topLeft) - sf::Vector2i(1, 1);


	// Adjust for corners
	if (m_tileMap[0][0].getBounds().left > topLeft.x)
		endIndex.x = 0;
	if (m_tileMap[0][0].getBounds().top > topLeft.y)
		endIndex.y = 0;

	// Adjust for corners
	if (m_tileMap[m_tileMap.size() - 1][m_tileMap[0].size() - 1].getBounds().left < bottomRight.x)
		startIndex.x = m_tileMap.size() - 1;
	if (m_tileMap[m_tileMap.size() - 1][m_tileMap[0].size() - 1].getBounds().top < bottomRight.y)
		startIndex.y = m_tileMap[0].size() - 1;

	// Clamp indexes
	startIndex.x = Utility::clampValue<int>(startIndex.x, 0, m_tileMap.size() - 1);
	startIndex.y = Utility::clampValue<int>(startIndex.y, 0, m_tileMap[m_tileMap.size() - 1].size() - 1);

	endIndex.x = Utility::clampValue<int>(endIndex.x, 0, m_tileMap.size() - 1);
	endIndex.y = Utility::clampValue<int>(endIndex.y, 0, m_tileMap[m_tileMap.size() - 1].size() - 1);

	// Render tiles from bottom right to top left
	for (int x = startIndex.x; x >= endIndex.x; x--)
		for (int y = startIndex.y; y >= endIndex.y; y--)
			m_tileMap[x][y].draw(false);
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