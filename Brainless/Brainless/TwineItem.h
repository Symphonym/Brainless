#ifndef INCLUDED_TWINE_ITEM_H
#define INCLUDED_TWINE_ITEM_H

#include "Item.h"

class TwineItem : public Item
{
public:

	TwineItem(int id);

	virtual Item* clone();

};

#endif