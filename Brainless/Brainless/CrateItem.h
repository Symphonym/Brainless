#ifndef INCLUDED_CRATE_ITEM_H
#define INCLUDED_CRATE_ITEM_H

#include "Item.h"

class CrateItem : public Item
{
public:
	CrateItem(int id);

	virtual void onExamine();

	virtual void draw();

	virtual Item* clone();
private:
	std::vector<int> m_itemsWithin;
};

#endif