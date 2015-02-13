#ifndef INCLUDED_THORS_HAMMER_ITEM_H
#define INCLUDED_THORS_HAMMER_ITEM_H

#include "Item.h"

class ThorsHammerItem : public Item
{
public:

	ThorsHammerItem(int id);

	virtual bool onInteract(Item &otherItem, Game &game);

	virtual Item* clone();
};

#endif