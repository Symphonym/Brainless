#ifndef INCLUDED_KEY_TWINE_ITEM_H
#define INCLUDED_KEY_TWINE_ITEM_H

#include "Item.h"

class KeyTwineItem : public Item
{
public:

	KeyTwineItem(int id);

	virtual Item* clone();
};

#endif