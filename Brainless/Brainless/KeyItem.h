#ifndef INCLUDED_KEY_ITEM_H
#define INCLUDED_KEY_ITEM_H

#include "Item.h"

class KeyItem : public Item
{
public:

	KeyItem(int id);

	virtual bool onInteract(Item &otherItem, Game &game);

	virtual Item* clone();
};

#endif