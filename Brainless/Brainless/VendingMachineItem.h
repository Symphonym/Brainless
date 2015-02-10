#ifndef INCLUDED_VENDING_MACHINE_ITEM_H
#define INCLUDED_VENDING_MACHINE_ITEM_H

#include "Item.h"

class VendingMachineItem : public Item
{
public:

	typedef std::vector<int> IdVector;

	VendingMachineItem(const std::string &textureName, const IdVector &itemsWithin, int id);

	virtual void onUse(Game &game);
	virtual bool onInteractedWith(Item &otherItem);

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	virtual Item* clone();

private:

	// A list of pending payments, TRUE = paid with coin, FALSE = cheated the system
	// There can only be 1 FALSE payment in the vector
	std::vector<bool> m_payments;
	IdVector m_itemsWithin;
};

#endif