#ifndef INCLUDED_NEWSPAPER_ITEM_H
#define INCLUDED_NEWSPAPER_ITEM_H

#include "Item.h"

class NewspaperItem : public Item
{
public:

	NewspaperItem(int id);

	virtual Item* clone();
};

#endif