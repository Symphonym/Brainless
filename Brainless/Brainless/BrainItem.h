#ifndef INCLUDED_BRAIN_ITEM_H
#define INCLUDED_BRAIN_ITEM_H

#include "Item.h"
#include "DialogTree.h"
#include "Game.h"

class BrainItem : public Item
{
public:

	BrainItem(int id);

	virtual bool onInteractUnit(Unit &unit, Game &game);

	virtual Item* clone();
private:
	DialogTree m_dialog;
};

#endif