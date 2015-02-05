#ifndef INCLUDED_INVENTORY_H
#define INCLUDED_INVENTORY_H

#include <SFML\Graphics.hpp>
#include "Constants.h"
#include "Item.h"
#include <memory>
#include <array>

class Level;
class Inventory
{
public:

	typedef std::unique_ptr<Item> ItemPtr;

	Inventory();

	// Adds an item to the inventory
	void addItem(ItemPtr item);

	// Set which level the inventory will interact with
	void setCurrentLevel(Level* level);

	void events(const sf::Event &event, const sf::RenderWindow &gameWindow, Level &level);
	void update(const sf::RenderWindow &gameWindow);

	void draw();

	bool holdingItem() const;

	// Returns a read-only list of all items in the inventory
	std::vector<const Item*> getInventoryItems() const;

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