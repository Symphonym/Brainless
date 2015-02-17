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

	static const int MapWidth = 40;
	static const int MapHeight = 15;

	static const int BlockTypeCount = 56;
	static const int ZombieTypeCount = 3;

	static const int InventoryWidth = 5;
	static const int InventoryHeight = 4;

	static const int InteractDistance = 300;
	static const int StickInteractAddition = 100;

	static const int SpiritFeatherWorth = 20;
	static const int SpiritGhostCost = 50;

	static const std::string CantPickUpString;
	static const std::string CantUseString;
};

#endif