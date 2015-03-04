#ifndef INCLUDED_BOOKSHELF_ITEM_H
#define INCLUDED_BOOKSHELF_ITEM_H

#include "Item.h"

class BookShelfItem : public Item
{
public:
	BookShelfItem(int id);

	virtual void onExamine();

	virtual void draw();

	virtual Item* clone();
private:
	std::vector<int> m_itemsWithin;
};

#endif