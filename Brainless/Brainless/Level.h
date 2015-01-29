#ifndef INCLUDED_LEVEL_H
#define INCLUDED_LEVEL_H

#include "TileMap.h"
#include "Item.h"
#include "LevelSprite.h"
#include "Unit.h"
#include <memory>
#include <vector>

class Level
{
public:

	typedef std::unique_ptr<TileMap> MapPtr;
	typedef std::unique_ptr<Item> ItemPtr;
	typedef std::unique_ptr<Unit> UnitPtr;

	Level();

	// Adds a unit to the level and returns a pointer to it
	Unit* addUnit(UnitPtr unit);


	void update(float deltaTime);
	void draw(const sf::View &cameraView);

	TileMap& getTileMap();
	std::vector<ItemPtr>& getItems();
	std::vector<LevelSprite>& getDecorations();
	std::vector<UnitPtr>& getUnits();

private:

	void updateUnitCollision(float deltaTime);

	MapPtr m_tileMap; // Tilemap of all the tiles in the level
	std::vector<ItemPtr> m_items; // Items in the level
	std::vector<LevelSprite> m_sprites; // Decoration sprites in the level
	std::vector<UnitPtr> m_units; // Units in the level


	// List of units
	// List of items
	// List of sprites (for decoration purposes)
};

#endif