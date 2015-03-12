#ifndef INCLUDED_LEVERSIDE_ITEM_H
#define INCLUDED_LEVERSIDE_ITEM_H

#include "Item.h"

class LeverSideItem : public Item
{
public:

	LeverSideItem(bool left, int id);

	virtual void onUse(Game &game);
	virtual void onExamine();

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	virtual void draw();

	virtual Item* clone();

private:

	bool m_isLeft;
};

#endif