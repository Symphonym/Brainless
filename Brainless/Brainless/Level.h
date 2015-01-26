#ifndef INCLUDED_LEVEL_H
#define INCLUDED_LEVEL_H

#include "TileMap.h"
#include "Item.h"
#include <SFML\Graphics.hpp>
#include <memory>
#include <vector>

class Level
{
public:

	typedef std::pair<sf::Sprite, bool> SpritePair; // Pairs of sprite, and whether or not it is drawn to the foreground (drawn to background otherwise)

	Level();

	void update(float deltaTime);
	void draw(const sf::View &cameraView);

	TileMap& getTileMap();
	std::vector<SpritePair>& getDecorations();

private:

	typedef std::unique_ptr<TileMap> MapPtr;
	typedef std::unique_ptr<Item> ItemPtr;

	MapPtr m_tileMap; // Tilemap of all the tiles in the level
	std::vector<ItemPtr> m_items; // Items in the level
	std::vector<SpritePair> m_sprites; // Decoration sprites in the level


	// List of units
	// List of items
	// List of sprites (for decoration purposes)
};

#endif