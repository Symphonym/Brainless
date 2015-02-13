#ifndef INCLUDED_BRAIN_ITEM_H
#define INCLUDED_BRAIN_ITEM_H

#include "Item.h"

class BrainItem : public Item
{
public:

	BrainItem(int id);

	virtual bool onInteractUnit(Item &otherItem, Game &game);

	virtual Item* clone();
};

#endif