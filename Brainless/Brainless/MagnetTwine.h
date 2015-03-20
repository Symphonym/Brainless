#ifndef INCLUDED_MAGNET_TWINE_H
#define INCLUDED_MAGNET_TWINE_H

#include "Item.h"

class MagnetTwine : public Item
{
public:

	MagnetTwine(int id);

	virtual Item* clone();
};

#endif