#ifndef INCLUDED_LONG_STICK_ITEM_H
#define INCLUDED_LONG_STICK_ITEM_H

#include "Item.h"

class LongStickItem : public Item
{
public:

	LongStickItem(int id);
	bool onInteract(Item &otherItem, Game &game);

	virtual Item* clone();
};

#endif