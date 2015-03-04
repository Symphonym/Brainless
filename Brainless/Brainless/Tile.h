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

		//ROAD
		Road_Top_Right = 6,
		Road_Top_Right_Corner = 56,

		Road_Top_Left = 7,
		Road_Top_Left_Short = 8, //
		Road_Top_Left_Corner = 55,

		Road_Top = 10,
		Road_Top_Alone = 11, 
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
		Road_Tilt_Corner = 57,

		Road_Corner_TopLeft = 25,
		Road_Corner_TopRight = 26,
		Road_Corner_TopLeft_TopRight = 27,
		Road_Corner_DownLeft = 29,
		Road_Corner_DownRight = 30,
		Road_Corner_DownLeft_DownRight = 31,
		Road_Corner_TopLeft_DownLeft = 32,
		Road_Corner_TopRight_DownRight = 33,
		Road_Corner_TopLeft_DownRight = 34,
		Road_Corner_TopRight_DownLeft = 35,
		Road_Corner_Not_TopLeft = 36,
		Road_Corner_Not_TopRight = 37,
		Road_Corner_Not_DownLeft = 38,
		Road_Corner_Not_DownRight = 39,
		Road_Corner_Full = 40,
		
		//WOOD
		Wood_Top_Right = 106,
		Wood_Top_Right_Corner = 156,

		Wood_Top_Left = 107,
		Wood_Top_Left_Short = 108, //
		Wood_Top_Left_Corner = 155,

		Wood_Top = 110,
		Wood_Top_Alone = 111, 
		Wood_Top_Short = 112, //
		Wood_Top_Corner_Left = 152,
		Wood_Top_Corner_Right = 153,
		Wood_Top_Corner_Full = 154,

		Wood_Right = 113,
		Wood_Right_Alone = 114,
		Wood_Right_Corner_Down = 149,
		Wood_Right_Corner_Top = 150,
		Wood_Right_Corner_Full = 151,

		Wood_Middle = 115,
		Wood_Middle_Alone = 116,
		Wood_Middle_Alone_Horizontal = 117,
		Wood_Middle_Alone_Vertical = 118,

		Wood_Left = 119,
		Wood_Left_Short = 120, //
		Wood_Left_Alone = 121,
		Wood_Left_Corner_Down = 146,
		Wood_Left_Corner_Top = 147,
		Wood_Left_Corner_Full = 148,

		Wood_Down_Right = 122,
		Wood_Down_Right_Corner = 145,
		Wood_Down_Left = 123,
		Wood_Down_Left_Corner = 144,

		Wood_Down = 124,
		Wood_Down_Alone = 128,
		Wood_Down_Corner_Left = 141,
		Wood_Down_Corner_Right = 142,
		Wood_Down_Corner_Full = 143,

		Wood_Tilt = 109,
		Wood_Tilt_Corner = 157,

		Wood_Corner_TopLeft = 125,
		Wood_Corner_TopRight = 126,
		Wood_Corner_TopLeft_TopRight = 127,
		Wood_Corner_DownLeft = 129,
		Wood_Corner_DownRight = 130,
		Wood_Corner_DownLeft_DownRight = 131,
		Wood_Corner_TopLeft_DownLeft = 132,
		Wood_Corner_TopRight_DownRight = 133,
		Wood_Corner_TopLeft_DownRight = 134,
		Wood_Corner_TopRight_DownLeft = 135,
		Wood_Corner_Not_TopLeft = 136,
		Wood_Corner_Not_TopRight = 137,
		Wood_Corner_Not_DownLeft = 138,
		Wood_Corner_Not_DownRight = 139,
		Wood_Corner_Full = 140,

		//GRASS
		Grass_Top_Right = 206,
		Grass_Top_Right_Corner = 256,

		Grass_Top_Left = 207,
		Grass_Top_Left_Short = 208, //
		Grass_Top_Left_Corner = 255,

		Grass_Top = 210,
		Grass_Top_Alone = 211,
		Grass_Top_Short = 212, //
		Grass_Top_Corner_Left = 252,
		Grass_Top_Corner_Right = 253,
		Grass_Top_Corner_Full = 254,

		Grass_Right = 213,
		Grass_Right_Alone = 214,
		Grass_Right_Corner_Down = 249,
		Grass_Right_Corner_Top = 250,
		Grass_Right_Corner_Full = 251,

		Grass_Middle = 215,
		Grass_Middle_Alone = 216,
		Grass_Middle_Alone_Horizontal = 217,
		Grass_Middle_Alone_Vertical = 218,

		Grass_Left = 219,
		Grass_Left_Short = 220, //
		Grass_Left_Alone = 221,
		Grass_Left_Corner_Down = 246,
		Grass_Left_Corner_Top = 247,
		Grass_Left_Corner_Full = 248,

		Grass_Down_Right = 222,
		Grass_Down_Right_Corner = 245,
		Grass_Down_Left = 223,
		Grass_Down_Left_Corner = 244,

		Grass_Down = 224,
		Grass_Down_Alone = 228,
		Grass_Down_Corner_Left = 241,
		Grass_Down_Corner_Right = 242,
		Grass_Down_Corner_Full = 243,

		Grass_Tilt = 209,
		Grass_Tilt_Corner = 257,

		Grass_Corner_TopLeft = 225,
		Grass_Corner_TopRight = 226,
		Grass_Corner_TopLeft_TopRight = 227,
		Grass_Corner_DownLeft = 229,
		Grass_Corner_DownRight = 230,
		Grass_Corner_DownLeft_DownRight = 231,
		Grass_Corner_TopLeft_DownLeft = 232,
		Grass_Corner_TopRight_DownRight = 233,
		Grass_Corner_TopLeft_DownRight = 234,
		Grass_Corner_TopRight_DownLeft = 235,
		Grass_Corner_Not_TopLeft = 236,
		Grass_Corner_Not_TopRight = 237,
		Grass_Corner_Not_DownLeft = 238,
		Grass_Corner_Not_DownRight = 239,
		Grass_Corner_Full = 240,

		//SEWER
		Sewer_Top_Right = 306,
		Sewer_Top_Right_Corner = 356,

		Sewer_Top_Left = 307,
		Sewer_Top_Left_Short = 308, //
		Sewer_Top_Left_Corner = 355,

		Sewer_Top = 310,
		Sewer_Top_Alone = 311,
		Sewer_Top_Short = 312, //
		Sewer_Top_Corner_Left = 352,
		Sewer_Top_Corner_Right = 353,
		Sewer_Top_Corner_Full = 354,

		Sewer_Right = 313,
		Sewer_Right_Alone = 314,
		Sewer_Right_Corner_Down = 349,
		Sewer_Right_Corner_Top = 350,
		Sewer_Right_Corner_Full = 351,

		Sewer_Middle = 315,
		Sewer_Middle_Alone = 316,
		Sewer_Middle_Alone_Horizontal = 317,
		Sewer_Middle_Alone_Vertical = 318,

		Sewer_Left = 319,
		Sewer_Left_Short = 320, //
		Sewer_Left_Alone = 321,
		Sewer_Left_Corner_Down = 346,
		Sewer_Left_Corner_Top = 347,
		Sewer_Left_Corner_Full = 348,

		Sewer_Down_Right = 322,
		Sewer_Down_Right_Corner = 345,
		Sewer_Down_Left = 323,
		Sewer_Down_Left_Corner = 344,

		Sewer_Down = 324,
		Sewer_Down_Alone = 328,
		Sewer_Down_Corner_Left = 341,
		Sewer_Down_Corner_Right = 342,
		Sewer_Down_Corner_Full = 343,

		Sewer_Tilt = 309,
		Sewer_Tilt_Corner = 357,

		Sewer_Corner_TopLeft = 325,
		Sewer_Corner_TopRight = 326,
		Sewer_Corner_TopLeft_TopRight = 327,
		Sewer_Corner_DownLeft = 329,
		Sewer_Corner_DownRight = 330,
		Sewer_Corner_DownLeft_DownRight = 331,
		Sewer_Corner_TopLeft_DownLeft = 332,
		Sewer_Corner_TopRight_DownRight = 333,
		Sewer_Corner_TopLeft_DownRight = 334,
		Sewer_Corner_TopRight_DownLeft = 335,
		Sewer_Corner_Not_TopLeft = 336,
		Sewer_Corner_Not_TopRight = 337,
		Sewer_Corner_Not_DownLeft = 338,
		Sewer_Corner_Not_DownRight = 339,
		Sewer_Corner_Full = 340,


		Solid_Invisible = 500
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
	bool getEditorVisibleOnly() const;

	// Get name of autotiling range
	std::string getAutotilingRangeName() const;

	// Raw acceess to sprite, do not modify if using tile with tilemap
	sf::Sprite& getSprite();

private:

	bool m_editorVisibleOnly;
	bool m_tilt;
	bool m_platform;
	std::string m_autotilingRange; // Name of autotiling range

	void updateType();

	TileTypes m_type;
	sf::Sprite m_tileSprite;

	sf::FloatRect m_bounds;
};

#endif