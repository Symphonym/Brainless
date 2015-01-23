#ifndef INCLUDED_TILE_H
#define INCLUDED_TILE_H

#include <SFML\Graphics.hpp>

class Tile
{
public:

	enum TileTypes
	{
		Nothing = 0,
		Ground = 1,
		Ground_Right = 2,
		// Etc
	};

	// Position/size of collision, tile type and offset for sprite
	explicit Tile(const sf::FloatRect &bounds, TileTypes tileType, const sf::Vector2f &spriteOffset);

	// Draw the tile sprite to the specified render target
	void draw(bool debug = false);

	// Checks if the tile collides with another bounding box
	bool collidesWith(const sf::FloatRect &rect);
	const sf::FloatRect& getBounds() const;

private:

	TileTypes m_type;
	sf::Sprite m_tileSprite;

	sf::FloatRect m_bounds;
};

#endif