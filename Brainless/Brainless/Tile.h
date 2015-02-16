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
		Road_Top_Right_Corner = 56,

		Road_Top_Left = 7,
		Road_Top_Left_Short = 8, //
		Road_Top_Left_Corner = 55,

		Road_Top = 10,
		Road_Top_Alone = 11, //fel bild, använder en "Road_Top_Alone_Short som vi inte har
		Road_Top_Short = 12, //
		Road_Top_Corner_Left = 52,
		Road_Top_Corner_Right = 53,
		Road_Top_Corner_Full = 54,

		Road_Right = 13,
		Road_Right_Alone = 14,
		Road_Right_Corner_Down = 49,
		Road_Right_Corner_Top = 50,
		Road_Right_Corner_Full = 51,

		Road_Middle = 15,
		Road_Middle_Alone = 16,
		Road_Middle_Alone_Horizontal = 17,
		Road_Middle_Alone_Vertical = 18,

		Road_Left = 19,
		Road_Left_Short = 20, //
		Road_Left_Alone = 21,
		Road_Left_Corner_Down = 46,
		Road_Left_Corner_Top = 47,
		Road_Left_Corner_Full = 48,

		Road_Down_Right = 22,
		Road_Down_Right_Corner = 45,
		Road_Down_Left = 23,
		Road_Down_Left_Corner = 44,

		Road_Down = 24,
		Road_Down_Alone = 28,
		Road_Down_Corner_Left = 41,
		Road_Down_Corner_Right = 42,
		Road_Down_Corner_Full = 43,

		Road_Tilt = 9,

		Road_Corner_TopLeft = 25,
		Road_Corner_TopRight = 26,
		Road_Corner_TopLeft_TopRight = 27,
		Road_Corner_DownLeft = 29,
		Road_Corner_DownRight = 30,
		Road_Corner_DownLeft_DownRight = 31,
		Road_Corner_TopLeft_DownLeft = 32,
		Road_Corner_TopRight_DownRight = 33, //MIA
		Road_Corner_TopLeft_DownRight = 34,
		Road_Corner_TopRight_DownLeft = 35,
		Road_Corner_Not_TopLeft = 36,
		Road_Corner_Not_TopRight = 37,
		Road_Corner_Not_DownLeft = 38,
		Road_Corner_Not_DownRight = 39,
		Road_Corner_Full = 40
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

	// Get name of autotiling range
	std::string getAutotilingRangeName() const;

	// Raw acceess to sprite, do not modify if using tile with tilemap
	sf::Sprite& getSprite();

private:
	bool m_tilt;
	bool m_platform;
	std::string m_autotilingRange; // Name of autotiling range

	void updateType();

	TileTypes m_type;
	sf::Sprite m_tileSprite;

	sf::FloatRect m_bounds;
};

#endif