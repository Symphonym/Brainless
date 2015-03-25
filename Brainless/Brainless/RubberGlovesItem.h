#ifndef INCLUDED_RUBBER_GLOVES_ITEM_H
#define INCLUDED_RUBBER_GLOVES_ITEM_H

#include "Item.h"

class RubberGlovesItem : public Item
{
public:

	RubberGlovesItem(int id);
	bool onInteract(Item &otherItem, Game &game);

	virtual Item* clone();
};

#endif