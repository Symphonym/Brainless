#ifndef INCLUDED_GAME_H
#define INCLUDED_GAME_H

#include <SFML\Graphics.hpp>
#include <memory>
#include <vector>

#include "Level.h"
#include "LevelTransition.h"
#include "Inventory.h"
#include "PopUpMenu.h"
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

	// Add item to inventory
	void lootItem(Inventory::ItemPtr item);

	// Loads a new level, resetting player position to starting position, resetting HUD etc
	void changeLevel(int levelIndex);
	// Does the same as 'changeLevel' except the screen will fade out and fade in
	void changeLevelTransition(int levelIndex);

	// Add a rendering camera, the list of extra cameras is cleared each frame
	void addCamera(const sf::View &camera);

	void saveGame();

	Player& getPlayer();
	Level& getLevel();

private:

	sf::View m_camera;
	std::vector<sf::View> m_extraCameras;

	std::unique_ptr<LevelTransition> m_levelTransition;

	int m_levelIndex;
	Level m_level;
	Player* m_player;

	// TESTING CODE, INVENTORY SHOULD PROBABLY BE IN PLAYER CLASS
	Inventory* m_inventory;
	// TESTING CODE, POPUP SHOULD PROBABLY BE IN MENU CLASS (MAYBE?!?!?!?)
	PopUpMenu* m_popup;
};

#endif