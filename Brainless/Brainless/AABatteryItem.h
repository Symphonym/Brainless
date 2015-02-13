#ifndef INCLUDED_AA_BATTERY_ITEM_H
#define INCLUDED_AA_BATTERY_ITEM_H

#include "Item.h"

class AABatteryItem : public Item
{
public:

	AABatteryItem(int id);

	virtual bool onInteract(Item &otherItem, Game &game);

	virtual Item* clone();
};

#endif