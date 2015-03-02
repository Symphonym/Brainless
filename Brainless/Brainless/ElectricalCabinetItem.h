#ifndef INCLUDED_ELECTRICALCABINET_ITEM_H
#define INCLUDED_ELECTRICALCABINET_ITEM_H

#include "Item.h"

class ElectricalCabinetItem : public Item
{
public:
	ElectricalCabinetItem(int id);

	virtual void onExamine();

	virtual void draw();

	virtual Item* clone();
};

#endif