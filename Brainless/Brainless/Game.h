#ifndef INCLUDED_GAME_H
#define INCLUDED_GAME_H

#include <SFML\Graphics.hpp>
#include <memory>
#include <vector>

#include "Level.h"
#include "Inventory.h"
#include "PopUpMenu.h"


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

	// Add item to inventory
	void lootItem(Inventory::ItemPtr item);

	// Loads a new level, resetting player position to starting position, resetting HUD etc
	void changeLevel(int levelIndex);

	void saveGame();

	Level& getLevel();
	const sf::RenderWindow& getWindow() const;

private:

	sf::RenderWindow m_game;
	sf::View m_camera;

	int m_levelIndex;
	Level m_level;
	Player* m_player;

	// TESTING CODE, INVENTORY SHOULD PROBABLY BE IN PLAYER CLASS
	Inventory* m_inventory;
	// TESTING CODE, POPUP SHOULD PROBABLY BE IN MENU CLASS (MAYBE?!?!?!?)
	PopUpMenu* m_popup;

	// Update loop
	void loop();

	// Rendering function
	void draw();
};

#endif