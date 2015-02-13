#ifndef INCLUDED_CAR_BATTERY_ITEM_H
#define INCLUDED_CAR_BATTERY_ITEM_H

#include "Item.h"

class CarBatteryItem : public Item
{
public:

	CarBatteryItem(int id);

	virtual bool onInteractedWith(Item &otherItem, Game &game);

	virtual Item* clone();
};

#endif