#ifndef INCLUDED_COIN_ITEM_H
#define INCLUDED_COIN_ITEM_H

#include "Item.h"

class CoinItem : public Item
{
public:

	CoinItem(int id);

	virtual bool onInteract(Item &otherItem);

	virtual Item* clone();
};

#endif