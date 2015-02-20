#ifndef INCLUDED_INVENTORY_H
#define INCLUDED_INVENTORY_H

#include <SFML\Graphics.hpp>
#include "Constants.h"
#include "Item.h"
#include "GUIElement.h"
#include <memory>
#include <array>

class Game;
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

	void events(const sf::Event &event, Game &game);
	void update(float deltaTime, Game &game);

	void draw();

	bool holdingItem() const;

	// Empties the inventory
	void emptyInventory();

	// Returns a read-only list of all items in the inventory
	std::vector<const Item*> getInventoryItems() const;

private:

	typedef std::unique_ptr<GUIElement> GuiPtr;
	typedef std::pair<ItemPtr, sf::Sprite> InventoryPair;

	// Gets the inventory slot at the specified location, null if there's none
	InventoryPair* getSlotAt(const sf::Vector2f &position);

	// If the inventory is open
	bool m_isOpen;

	// Item currently selected on the mouse
	ItemPtr m_mouseItem;

	sf::Text m_highlightText;
	sf::Texture m_highlightBackground;
	sf::Sprite m_highlightBGSprite;
	bool m_showHighlighText;

	void craft();
	void setCraftingMode(bool enabled);
	bool m_craftingModeEnabled;
	sf::Text m_craftText;
	GuiPtr m_craftButton;

	std::array<std::array<InventoryPair, Constants::InventoryHeight>, Constants::InventoryWidth> m_slots;

};

#endif