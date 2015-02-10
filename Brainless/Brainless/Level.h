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

	void setSpawnPosition(const sf::Vector2f &spawnPos);

	// Adds a unit to the level and returns a pointer to it
	Unit* addUnit(UnitPtr unit);

	void addItem(ItemPtr item);
	void addDecoration(const LevelSprite &decoration);

	// Removes an item (if found) from the level and returns it
	// The item will be destroyed if the return value isn't used
	ItemPtr removeItem(Item *item);
	// Remove item by index, much faster than the above alternative
	ItemPtr removeItem(std::size_t index);
	
	void removeAllItems();

	void removeDecoration(std::size_t index);
	void removeUnit(std::size_t index);
	// Reset everything in the level
	void reset();

	void update(float deltaTime, Game &game);
	void draw(const sf::View &cameraView);

	TileMap& getTileMap();
	const std::vector<ItemPtr>& getItems() const;
	const std::vector<LevelSprite>& getDecorations() const;
	const std::vector<UnitPtr>& getUnits() const;

	// Get non-const access to level stuff
	Item& getItem(std::size_t index);
	Unit& getUnit(std::size_t index);

	const sf::Vector2f& getSpawnPos() const;

private:

	void updateUnitCollision(float deltaTime);

	sf::Vector2f m_spawnPos;

	MapPtr m_tileMap; // Tilemap of all the tiles in the level
	std::vector<ItemPtr> m_items; // Items in the level
	std::vector<LevelSprite> m_sprites; // Decoration sprites in the level
	std::vector<sf::Sprite> m_backgrounds; //Backgrounds in the level
	std::vector<UnitPtr> m_units; // Units in the level


	// List of units
	// List of items
	// List of sprites (for decoration purposes)
};

#endif