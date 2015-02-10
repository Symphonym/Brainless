#ifndef INCLUDED_COIN_TWINE_ITEM_H
#define INCLUDED_COIN_TWINE_ITEM_H

#include "Item.h"

class CoinTwineItem : public Item
{
public:

	CoinTwineItem(int id);

	bool onInteract(Item &otherItem);

	virtual Item* clone();
};

#endif