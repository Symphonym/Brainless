#ifndef INCLUDED_CONSTANTS_H
#define INCLUDED_CONSTANTS_H

#include <string>

class Constants
{
public:

	static const int Gravity = 800;

	static const int TileSize = 96;
	static const int LeftTileOffset = -20;
	static const int TopTileOffset = -20;

	static const int MapWidth = 200;
	static const int MapHeight = 50;

	static const int BlockTypeCount = 5;

	static const int InventoryWidth = 5;
	static const int InventoryHeight = 4;

	static const int InteractDistance = 150;

	static const std::string CantPickUpString;
	static const std::string CantUseString;
};

#endif