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

	static const int BlockTypeCount = 258;
	static const int ZombieTypeCount = 3;
	static const int ZombieTextureCount = 7;

	static const int InventoryWidth = 4;
	static const int InventoryHeight = 4;

	static const int InteractDistance = 220;
	static const int StickInteractAddition = 100;

	static const int SpiritFeatherWorth = 13;
	static const int SpiritGhostCost = 26;

	// Must never be zero, and must equal the amount of brains placed in the levels
	static const int TotalBrainCount = 7;

	static const std::string CantPickUpString;
	static const std::string CantUseString;

	static const std::string NoMusicProvidedString;

	// If spirit is less than this, a tutorial/help dialog will show the first time it happens
	static const int SpiritHelpTriggerAmount = SpiritGhostCost;
};

#endif