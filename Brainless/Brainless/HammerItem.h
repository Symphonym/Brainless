#ifndef INCLUDED_HAMMER_ITEM_H
#define INCLUDED_HAMMER_ITEM_H

#include "Item.h"

class HammerItem : public Item
{
public:

	HammerItem(int id);

	virtual bool onInteract(Item &otherItem, Game &game);

	virtual Item* clone();
};

#endif