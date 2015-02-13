#ifndef INCLUDED_USABLE_ITEM_H
#define INCLUDED_USABLE_ITEM_H

#include "Item.h"

class PokableItem : public Item
{
public:

	PokableItem(const std::string &itemName, const std::string &textureName, int id);

	virtual bool onInteractedWith(Item &otherItem, Game &game);

	// When an item is poked with stick
	virtual void onPoked(Game &game) {};

};

#endif