#ifndef INCLUDED_CABLE_ITEM_H
#define INCLUDED_CABLE_ITEM_H

#include "Item.h"

class CableItem : public Item
{
public:

	CableItem(int id);

	virtual bool onInteract(Item &otherItem, Game &game);

	virtual Item* clone();
};

#endif