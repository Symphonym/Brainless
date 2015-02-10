#ifndef INCLUDED_STONE_ITEM_H
#define INCLUDED_STONE_ITEM_H

#include "Item.h"

class StoneItem : public Item
{
public:

	StoneItem(int id);

	virtual Item* clone();
};

#endif