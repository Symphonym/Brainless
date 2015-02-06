#ifndef INCLUDED_CHEST_ITEM_H
#define INCLUDED_CHEST_ITEM_H

#include "Item.h"

class ChestItem : public Item
{
public:

	ChestItem(bool locked, std::vector<int> itemsWithin, int id);

	virtual void onUse(Game &game);
	virtual bool onInteractedWith(Item &otherItem);

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	virtual Item* clone();

private:

	// Items in the chest (ID's)
	std::vector<int> m_itemsWithin;

	bool m_isLocked;
	bool m_isOpen;
};

#endif