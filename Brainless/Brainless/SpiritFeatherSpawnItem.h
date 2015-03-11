#ifndef INCLUDED_SPIRIT_FEATHER_SPAWN_ITEM_H
#define INCLUDED_SPIRIT_FEATHER_SPAWN_ITEM_H

#include "Item.h"

class SpiritFeatherSpawnItem : public Item
{
public:

	SpiritFeatherSpawnItem(int id);

	virtual void update(float deltaTime, Game &game);

	virtual Item* clone();

private:
};

#endif