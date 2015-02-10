#ifndef INCLUDED_DUCT_TAPE_ITEM_H
#define INCLUDED_DUCT_TAPE_ITEM_H

#include "Item.h"

class DuctTapeItem : public Item
{
public:

	DuctTapeItem(int id);


	virtual Item* clone();
};

#endif