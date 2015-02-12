#ifndef INCLUDED_VENDING_MACHINE_ITEM_H
#define INCLUDED_VENDING_MACHINE_ITEM_H

#include "Item.h"

class VendingMachineItem : public Item
{
public:

	typedef std::vector<int> IdVector;

	VendingMachineItem(const std::string &textureName, const IdVector &itemsWithin, int id);

	virtual bool onInteractedWith(Item &otherItem, Game &game);

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	virtual Item* clone();

private:

	IdVector m_itemsWithin;
};

#endif