#pragma once
#include "Level.h"

class FileSave
{
public:
	//Saving & loading levels
	static void saveMap(Level* stage, int stage_number);
	static void loadMap(Level* stage, int stage_number);

	// Text saving
	static void saveMapText(Level &level, int levelNumber);

	// Returns true if the file was loaded
	static bool loadMapText(Level &level, int levelNumber);

	//Saving & loading player progress and misc info
	static void saveProgress();
	static void loadProgress();
};