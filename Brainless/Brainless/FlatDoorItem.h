#ifndef INCLUDED_FLATDOOR_ITEM_H
#define INCLUDED_FLATDOOR_ITEM_H
#include "Item.h"

class FlatDoorItem : public Item
{
public:

	FlatDoorItem(bool locked, int id);

	// To handle unlocking of the door, if it is locked
	virtual bool onInteractedWith(Item &otherItem, Game &game);

	// To handle opening of door, if it is unlocked
	virtual void onUse(Game &game);
	virtual void onExamine();

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	virtual void draw();

	virtual Item* clone();

private:
	bool m_isOpen;
	bool m_isLocked;

};

#endif