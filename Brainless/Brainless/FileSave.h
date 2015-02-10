#pragma once
#include "Level.h"

class Inventory;
class FileSave
{
public:
	// Text saving
	static void saveMapText(Level &level, int levelNumber);

	// Returns true if the file was loaded
	static bool loadMapText(Level &level, int levelNumber);

	static void saveInventory(Inventory &inventory);
	static bool loadInventory(Inventory &inventory);

	static void saveLevelProgress(Level &level, int levelNumber);
	static bool loadLevelProgress(Level &level, int levelNumber);

	static void wipeProgress();

	//Saving & loading player progress and misc info
	//static void saveProgress();
	//static void loadProgress();
};