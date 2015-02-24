#ifndef INCLUDED_MANHOLE_ITEM_H
#define INCLUDED_MANHOLE_ITEM_H

#include "Item.h"

class ManholeItem : public Item
{
public:

	ManholeItem(bool isOpen, int id);

	virtual void onUse(Game &game);

	//virtual void draw();

	virtual Item* clone();

private:
	bool m_open;
};

#endif