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
		Platform = 5,
		//Short är tales som angränsar en annan tile "snett"
		//Alone är tiles som inte angränsar till något verticalt/horiontalt
		Road_Top_Right = 6,
		Road_Top_Left = 7,
		Road_Top_Left_Short = 8,
		Road_Tilt = 9,
		Road_Top = 10,
		Road_Top_Alone = 11,
		Road_Top_Short = 12,
		Road_Right = 13,
		Road_Right_Alone = 14,
		Road_Middle = 15,
		Road_Middle_Alone = 16,
		Road_Middle_Alone_Horizontal = 17,
		Road_Middle_Alone_Vertical = 18, //missing in action?
		Road_Left = 19,
		Road_Left_Short = 20,
		Road_Left_Alone = 21,
		Road_Down_Right = 22,
		Road_Down_Left = 23, 
		Road_Down = 24,
		Road_Corner_Left = 25,
		Road_Corner_Right = 26

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
	bool m_tilt;
	bool m_platform;
	void updateType();

	TileTypes m_type;
	sf::Sprite m_tileSprite;

	sf::FloatRect m_bounds;
};

#endif