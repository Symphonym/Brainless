#ifndef INCLUDED_GAME_H
#define INCLUDED_GAME_H

#include <SFML\Graphics.hpp>
#include <memory>
#include <vector>

#include "Level.h"
#include "Inventory.h"


class Level;
class Unit;
class Player;

class Game
{
public:
	
	Game();
	~Game();
	
	// starts the game
	void run();

private:

	sf::RenderWindow m_game;
	sf::View m_camera;

	Level m_level;
	Player* m_player;
	Inventory* m_inventory;

	// Load/save functionality
	void loadFile();
	void saveFile();

	// Update loop
	void loop();

	// Rendering function
	void draw();
};

#endif