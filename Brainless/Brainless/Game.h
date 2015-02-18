#ifndef INCLUDED_GAME_H
#define INCLUDED_GAME_H

#include <SFML\Graphics.hpp>
#include <memory>
#include <vector>

#include "Level.h"
#include "LevelTransition.h"
#include "Inventory.h"
#include "PopUpMenu.h"
#include "SpiritBar.h"
#include "State.h"


class Level;
class Unit;
class Player;

class Game : public State
{
public:
	
	Game(StateMachine &machine);
	~Game();
	
	virtual void events(const sf::Event &event);
	virtual void update(float deltaTime);
	virtual void draw();

	// Increment the saved zombie amount
	void addSavedZombie(int count);

	// Add item to inventory
	void lootItem(Inventory::ItemPtr item);

	// Loads a new level, resetting player position to starting position, resetting HUD etc
	void changeLevel(int levelIndex,bool swapPosition);
	// Does the same as 'changeLevel' except the screen will fade out and fade in
	void changeLevelTransition(int levelIndex,bool swapPosition);

	// Add a rendering camera, the list of extra cameras is cleared each frame
	void addCamera(const sf::View &camera);

	void saveGame();

	Player& getPlayer();
	Level& getLevel();
	SpiritBar& getSpiritBar();

	// Check if the inventory contains a specific item
	bool inventoryContains(const std::string &itemName);

private:

	sf::View m_camera;
	std::vector<sf::View> m_extraCameras;

	std::unique_ptr<LevelTransition> m_levelTransition;

	// How many zombies has been given a brain
	int m_savedZombies;

	int m_levelIndex;
	Level m_level;
	Player* m_player;

	Inventory* m_inventory;
	PopUpMenu* m_popup;
	SpiritBar* m_spiritBar;
};

#endif