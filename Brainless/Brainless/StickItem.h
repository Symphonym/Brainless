#ifndef INCLUDED_STICK_ITEM_H
#define INCLUDED_STICK_ITEM_H

#include "Item.h"

class StickItem : public Item
{
public:
	
	StickItem(int id);

	virtual Item* clone();
};


#endif