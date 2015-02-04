#ifndef INCLUDED_DOOR_ITEM_H
#define INCLUDED_DOOR_ITEM_H

#include "Item.h"

class DoorItem : public Item
{
public:

	DoorItem(bool locked, int id);

	// To handle unlocking of the door, if it is locked
	virtual bool onInteractedWith(Item &otherItem);

	// To handle opening of door, if it is unlocked
	virtual void onUse(const sf::RenderWindow &window);
	virtual void onExamine();

	virtual void draw();

	virtual Item* clone();

private:

	bool m_isOpen;
	bool m_isLocked;

};

#endif