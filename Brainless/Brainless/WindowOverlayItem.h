#ifndef INCLUDED_WINDOWOVERLAY_ITEM_H
#define INCLUDED_WINDOWOVERLAY_ITEM_H

#include "Item.h"

class WindowOverlayItem : public Item
{
public:

	WindowOverlayItem(int id);

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	virtual bool onInteractedWith(Item &otherItem, Game &game);

	virtual Item* clone();

private:
};

#endif