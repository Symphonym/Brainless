#ifndef INCLUDED_LEVEL_H
#define INCLUDED_LEVEL_H

#include "TileMap.h"
#include "Item.h"
#include "LevelSprite.h"
#include <SFML\Graphics.hpp>
#include <memory>
#include <vector>

class Level
{
public:

	typedef std::unique_ptr<TileMap> MapPtr;
	typedef std::unique_ptr<Item> ItemPtr;

	Level();

	void update(float deltaTime);
	void draw(const sf::View &cameraView);

	TileMap& getTileMap();
	std::vector<ItemPtr>& getItems();
	std::vector<LevelSprite>& getDecorations();

private:

	MapPtr m_tileMap; // Tilemap of all the tiles in the level
	std::vector<ItemPtr> m_items; // Items in the level
	std::vector<LevelSprite> m_sprites; // Decoration sprites in the level


	// List of units
	// List of items
	// List of sprites (for decoration purposes)
};

#endif