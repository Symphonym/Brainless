#pragma once
#include "TileMap.h"

class FileSave
{
public:
	//Saving & loading levels
	static void saveMap(TileMap* map, int stage_number);
	static void loadMap(TileMap* map, int stage_number);

	//Saving & loading player progress and misc info
	static void saveProgress();
	static void loadProgress();
};