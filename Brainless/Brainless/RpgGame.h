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

	static const int MapWidth = 15;
	static const int MapHeight = 15;

	void spawnPlayer();
	void createWave();
	void tickEnemies();

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

	typedef std::unique_ptr<TileUnit> UnitPtr;

	void playerInputToTile(int x, int y);
	bool canMove(int x, int y);
	bool canAttack(int x, int y); // Only usable by player unit

	void placeUnitOnTile(TileUnit *unit, int x, int y);
	TileUnit& spawnUnit(const std::string &textureName);
	std::vector<sf::Vector2i> getAdjacentIndices(int x, int y) const;
	void removeUnit(TileUnit *unit);



	std::vector<UnitPtr> m_units;
	TileUnit *m_player;

	std::array<std::array<Tile, MapHeight>, MapWidth> m_tiles;
};

#endif