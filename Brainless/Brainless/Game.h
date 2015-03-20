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
#include "HealthBar.h"
#include "State.h"

class GUIElement;
class Level;
class Unit;
class Player;

class Game : public State
{
public:
	
	Game(StateMachine &machine);
	~Game();
	
	virtual void onPlay();
	virtual void onStop();

	virtual void events(const sf::Event &event);
	virtual void update(float deltaTime);
	virtual void draw();

	// Increment the saved zombie amount
	void addSavedZombie(int count);
	void clearSavedZombies();

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
	int getLevelIndex();
	SpiritBar& getSpiritBar();
	int getSavedZombieCount() const;

	// Check if the inventory contains a specific item
	bool inventoryContains(const std::string &itemName) const;

	// If the player is holding an item from the inventory, about to interact with the world
	bool holdingItem() const;

private:

	sf::View m_camera;
	std::vector<sf::View> m_extraCameras;

	std::unique_ptr<LevelTransition> m_levelTransition;
	std::unique_ptr<GUIElement> m_inventoryButton;

	// How many zombies has been given a brain
	int m_savedZombies;

	int m_levelIndex;
	Level m_level;
	Player* m_player;

	Inventory* m_inventory;
	PopUpMenu* m_popup;
	SpiritBar* m_spiritBar;
	HealthBar* m_healthBar;
};

#endif