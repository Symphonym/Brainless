#ifndef INCLUDED_RPG_GAME_H
#define INCLUDED_RPG_GAME_H

#include "ArcadeGame.h"
#include <SFML\Graphics.hpp>
#include <memory>
#include <array>
#include <vector>
#include <string>

class RpgGame : public ArcadeGame
{
public:

	RpgGame(ArcadeMachine &machine);

	virtual void onGameStart();

	virtual void events(const sf::Event &event);
	virtual void update(float deltaTime);
	virtual void draw();

private:

	static const int MapWidth = 8;
	static const int MapHeight = 8;


	struct TileUnit
	{
		sf::Sprite sprite;
		int x, y; // Tile index
	};
	struct Tile
	{
		sf::Sprite sprite;
		TileUnit *unit;
	};

	// Destination tile and the unit
	typedef std::unique_ptr<TileUnit> UnitPtr;

	sf::Text m_infoText;

	sf::Text m_gameOverText;

	int m_score;
	sf::Text m_scoreText;

	int m_hunger;
	const int m_hungerMax = 300;
	float m_hungerDelay;
	sf::Texture m_hungerBarTexture;
	sf::Sprite m_hungerBar;

	int m_spawnDelayCur;
	int m_spawnDelayMax;

	void spawnPlayer();
	void spawnEnemy(const std::string &textureName);
	void tickGame();

	void playerInputToTile(int x, int y);
	bool canMove(int x, int y);
	bool canAttack(int x, int y); // Only usable by player unit

	void placeUnitOnTile(TileUnit *unit, int x, int y);
	void removeUnit(TileUnit *unit, const sf::Color &particleColor);

	std::vector<UnitPtr> m_units;
	TileUnit *m_player;

	std::array<std::array<Tile, MapHeight>, MapWidth> m_tiles;
};

#endif