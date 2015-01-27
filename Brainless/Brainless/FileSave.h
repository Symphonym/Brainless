#pragma once
#include "Level.h"

class FileSave
{
public:
	//Saving & loading levels
	static void saveMap(Level* stage, int stage_number);
	static void loadMap(Level* stage, int stage_number);

	//Saving & loading player progress and misc info
	static void saveProgress();
	static void loadProgress();
};