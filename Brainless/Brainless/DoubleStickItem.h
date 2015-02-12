#ifndef INCLUDED_DOUBLE_STICK_ITEM_H
#define INCLUDED_DOUBLE_STICK_ITEM_H

#include "Item.h"

class DoubleStickItem : public Item
{
public:

	DoubleStickItem(int id);

	virtual Item* clone();
};

#endif