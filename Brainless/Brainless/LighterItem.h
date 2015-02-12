#ifndef INCLUDED_LIGHTER_ITEM_H
#define INCLUDED_LIGHTER_ITEM_H

#include "Item.h"

class LighterItem : public Item
{
public:

	LighterItem(int id);

	virtual void heldUpdate(float deltaTime, Game &game);
	virtual void update(float deltaTime, Game &game);

	virtual Item* clone();

private:
};

#endif