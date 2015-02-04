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
		Red = 2,
		Blue = 3,
		Tilt = 4,
		Platform = 5
		// Etc
	};

	// Position/size of collision, tile type and offset for sprite
	explicit Tile(const sf::FloatRect &bounds, TileTypes tileType, const sf::Vector2f &spriteOffset);

	void setType(TileTypes type);

	// Draw the tile sprite
	void draw();

	// Checks if the tile collides with another bounding box
	bool collidesWith(const sf::FloatRect &rect) const;

	const sf::FloatRect& getBounds() const;
	TileTypes getType() const;
	bool getTilt() const;
	bool getPlatform() const;

	// Raw acceess to sprite, do not modify if using tile with tilemap
	sf::Sprite& getSprite();

private:

	void updateType();

	TileTypes m_type;
	sf::Sprite m_tileSprite;

	sf::FloatRect m_bounds;
};

#endif