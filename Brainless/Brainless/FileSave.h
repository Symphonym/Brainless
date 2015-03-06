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

	static void saveGameData(Game &game);
	static void loadGameData(Game &game);

	// Searches for save files and returns the highest index of these files
	// Basically it's just to see how many levels you have played so you can
	// load progress from the level you're currently at and not level 0 by default.
	static int findHighestSavedLevelIndex();

	static void wipeProgress();

	//Saving & loading player progress and misc info
	//static void saveProgress();
	//static void loadProgress();
};