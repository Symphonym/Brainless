#ifndef INCLUDED_TILEMAP_H
#define INCLUDED_TILEMAP_H

#include "Tile.h"
#include <vector>

class TileMap
{
public:

	typedef std::vector<std::vector<Tile::TileTypes> > TileMapLayout;

	// Initialize a map of 'width' times 'height' tiles
	explicit TileMap(TileMapLayout layout, const unsigned int tileSize);

	// Draws the tile grid relative to a camera
	//void draw(const sf::View &view);

	// Converts a position to an index in the grid
	sf::Vector2i positionToIndex(const sf::Vector2f &position);

	// Get the tile at the specified coordinates
	Tile &getTile(int x, int y);

private:

	typedef std::vector<Tile> TileVector;

	const unsigned int m_tileSize;
	std::vector<TileVector> m_tileMap;

};

#endif