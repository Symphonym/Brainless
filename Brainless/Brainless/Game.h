#pragma once
#include <SFML\Graphics.hpp>
#include <memory>
#include <vector>

#include "level.h"


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

	// Load/save functionality
	void loadFile();
	void saveFile();

	// Update loop
	void loop();

	// Rendering function
	void draw();

	//Temprary marker 
	sf::Sprite m_markerSprite;
	sf::Texture m_markerTexture;
};

