#ifndef INCLUDED_LEVER_ITEM_H
#define INCLUDED_LEVER_ITEM_H

#include "Item.h"

class LeverItem : public Item
{
public:

	LeverItem(bool down, int id);

	virtual void onUse(Game &game);
	virtual void onExamine();
	virtual bool onInteractedWith(Item &otherItem, Game &game);

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	virtual void draw();

	virtual Item* clone();

private:

	bool m_isDown;
};

#endif