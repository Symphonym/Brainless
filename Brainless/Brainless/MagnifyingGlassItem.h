#ifndef INCLUDED_MAGNIFYING_GLASS_H
#define INCLUDED_MAGNIFYING_GLASS_H

#include <SFML\Graphics.hpp>
#include "Item.h"

class MagnifyingGlassItem : public Item
{
public:

	MagnifyingGlassItem(int id);

	virtual void heldUpdate(float deltaTime, Game &game);
	virtual void heldDraw();

	virtual Item* clone();

private:

	sf::Sprite m_background;
	sf::View m_magnifyingView;

};

#endif