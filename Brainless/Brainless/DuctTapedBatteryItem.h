#ifndef INCLUDED_DUCT_TAPED_BATTERY_ITEM_H
#define INCLUDED_DUCT_TAPED_BATTERY_ITEM_H

#include "Item.h"

class DuctTapedBatteryItem : public Item
{
public:

	DuctTapedBatteryItem(int id);

	virtual Item* clone();
};

#endif