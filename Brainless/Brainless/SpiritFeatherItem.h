#ifndef INCLUDED_SPIRIT_FEATHER_ITEM_H
#define INCLUDED_SPIRIT_FEATHER_ITEM_H

#include "Item.h"

class SpiritFeatherItem : public Item
{
public:

	SpiritFeatherItem(int id);

	virtual bool onCollisionWithUnit(Unit &unit, Game &game);
	virtual void draw();

	virtual Item* clone();
};

#endif