#ifndef INCLUDED_EMPTY_BAG_ITEM_H
#define INCLUDED_EMPTY_BAG_ITEM_H

#include "Item.h"

class EmptyBagItem : public Item
{
public:

	EmptyBagItem(int id);

	virtual Item* clone();
};

#endif