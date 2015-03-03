#ifndef INCLUDED_STUFFED_PAPER_BAG_ITEM_H
#define INCLUDED_STUFFED_PAPER_BAG_ITEM_H

#include "Item.h"

class StuffedPaperBagItem : public Item
{
public:

	StuffedPaperBagItem(int id);

	virtual bool onInteract(Item &otherItem, Game &game);

	virtual Item* clone();
};

#endif