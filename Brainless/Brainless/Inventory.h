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

	void events(const sf::Event &event, Game &game);
	void update(float deltaTime, Game &game);

	void draw();

	bool holdingItem() const;

	void toggleVisible();

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
	InventoryPair* m_mouseItemSlot;

	sf::Text m_highlightText;
	sf::Texture m_highlightBackground;
	sf::Sprite m_highlightBGSprite;
	sf::Sprite m_background;
	bool m_showHighlighText;

	void recolorSlots(const sf::Color &color);
	void highlightSelected();
	void highlightCraftables();

	void craft();
	void setCraftingMode(bool enabled);
	bool m_craftingModeEnabled;
	GuiPtr m_craftButton;
	std::vector<sf::Vector2i> m_selectedSlots;

	std::array<std::array<InventoryPair, Constants::InventoryHeight>, Constants::InventoryWidth> m_slots;

	bool m_lighterInventory;

};

#endif