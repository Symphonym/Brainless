#pragma once
#include <vector>

#include "Level.h"
#include "Unit.h"

class Game
{
public:
	Game();
private:
	Level* stage;
	std::vector<Unit*> enteties;
};

