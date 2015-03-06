#ifndef INCLUDED_LAMP_ITEM_H
#define INCLUDED_LAMP_ITEM_H

#include "Item.h"

class Lamp : public Item
{
public:
	Lamp(int id);

	virtual void onExamine();

	virtual void draw();

	virtual Item* clone();
private:
	std::vector<int> m_itemsWithin;
};

#endif