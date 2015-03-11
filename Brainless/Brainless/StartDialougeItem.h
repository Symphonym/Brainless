#ifndef INCLUDED_START_DIALOUGE_ITEM_H
#define INCLUDED_START_DIALOUGE_ITEM_H

#include "Item.h"

class StartDialougeItem : public Item
{
public:

	StartDialougeItem(int id);

	virtual bool onCollisionWithUnit(Unit &unit, Game &game);
	virtual void draw() {};
	virtual Item* clone();
};

#endif