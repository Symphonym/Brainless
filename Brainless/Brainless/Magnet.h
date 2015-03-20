#ifndef INCLUDED_MAGNET_H
#define INCLUDED_MAGNET_H

#include "item.h"

class Magnet : public Item
{
public:

	Magnet(int id);

	virtual Item* clone();
};

#endif