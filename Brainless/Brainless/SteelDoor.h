#ifndef INCLUDED_STEEL_DOOR_ITEM_H
#define INCLUDED_STEEL_DOOR_ITEM_H
#include "Item.h"

class SteelDoor : public Item
{
public:

	SteelDoor(bool locked, int id);

	// To handle unlocking of the door, if it is locked
	virtual bool onInteractedWith(Item &otherItem, Game &game);

	// To handle opening of door, if it is unlocked
	virtual void onUse(Game &game);
	virtual void onExamine();
	virtual bool onSyncedWith(Item &otherItem);

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	virtual void draw();

	virtual Item* clone();

private:
	bool m_isOpen;
	bool m_isLocked;

};

#endif