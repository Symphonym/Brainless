#ifndef INCLUDED_INVENTORY_H
#define INCLUDED_INVENTORY_H

#include <SFML\Graphics.hpp>
#include "Constants.h"
#include "Item.h"
#include "Level.h"
#include <memory>
#include <array>

class Inventory
{
public:

	typedef std::unique_ptr<Item> ItemPtr;

	Inventory();

	// Adds an item to the inventory
	void addItem(ItemPtr item);

	void events(const sf::Event &event, const sf::RenderWindow &gameWindow, Level &level);
	void update(const sf::RenderWindow &gameWindow);

	void draw();

private:

	typedef std::pair<ItemPtr, sf::Sprite> InventoryPair;

	// Gets the inventory slot at the specified location, null if there's none
	InventoryPair* getSlotAt(const sf::Vector2f &position);

	// If the inventory is open
	bool m_isOpen;

	// Item currently selected on the mouse
	ItemPtr m_mouseItem;

	std::array<std::array<InventoryPair, Constants::InventoryHeight>, Constants::InventoryWidth> m_slots;

};

#endif