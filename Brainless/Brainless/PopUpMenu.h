#ifndef INCLUDED_POP_UP_MENU_H
#define INCLUDED_POP_UP_MENU_H

#include <SFML\Graphics.hpp>
#include <array>
#include <functional>
#include <bitset>
#include "Unit.h"
#include "Item.h"

class Level;
class PopUpMenu
{
public:

	enum InteractTypes
	{
		Nothing,
		Use,
		Pickup,
		Examine
	};

	// If this returns true, then the item will be removed from the level
	// and the memory of the item must be managed.
	typedef std::function<void(Item*, InteractTypes)> ItemCallback;

	PopUpMenu();

	void setPosition(const sf::Vector2f &pos);
	void setItemCallback(ItemCallback callback);

	void events(const sf::Event &event, Game &game);
	void update(Game &game, const sf::Vector2f &playerOrigo);
	void draw();

private:

	static const int UseIndex = 0;
	static const int PickupIndex = 1;
	static const int ExamineIndex = 2;

	Item *m_interactItem;
	ItemCallback m_itemInteractCallback;

	bool m_isShowing;

	// Base position of the popup menu
	sf::Vector2f m_position;

	std::array<sf::Sprite, 3> m_buttons;
	sf::Sprite m_background;
};

#endif