#pragma once
#include <memory>
#include <vector>

#include "level.h"


class Level;
class Unit;

class Game
{
public:
	Game();
	//void run();
private:
	Level m_level;
	std::vector<Unit*> units;
	
	// Load/save functionality
	/*void loadFile();
	void saveFile();

	// Update loop
	void loop();

	// Rendering function
	void draw();*/
};

