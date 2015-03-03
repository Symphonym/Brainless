#ifndef INCLUDED_ELECTRICALCABINET_ITEM_H
#define INCLUDED_ELECTRICALCABINET_ITEM_H

#include "Item.h"

class ElectricalCabinetItem : public Item
{
public:
	ElectricalCabinetItem(std::vector<int> itemsWithin, int id);

	virtual void onExamine();

	bool onCollisionWithUnit(Unit &unit, Game &game);

	virtual int getScriptUnitType();

	virtual void draw();

	virtual Item* clone();
private:
	std::vector<int> m_itemsWithin;
};

#endif