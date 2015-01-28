#pragma once
#include <SFML\Graphics.hpp>
#include <memory>
#include <vector>

#include "level.h"


class Level;
class Unit;

class Game
{
public:
	
	Game();
	~Game();
	
	// starts the game
	void run();

private:

	sf::RenderWindow m_game;
	sf::View m_camera, m_hudCamera;

	Level m_level;
	std::vector<Unit*> units;

	// Load/save functionality
	void loadFile();
	void saveFile();

	// Update loop
	void loop();

	// Rendering function
	void draw();
};

