#ifndef INCLUDED_ARCADE_MACHINE_ITEM_H
#define INCLUDED_ARCADE_MACHINE_ITEM_H

#include "Item.h"

class ArcadeMachineItem : public Item
{
public:

	ArcadeMachineItem(int id);

	virtual void onUse(Game &game);

	virtual Item* clone();

};

#endif