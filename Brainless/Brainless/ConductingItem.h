#ifndef INCLUDED_CONDUCTING_ITEM_H
#define INCLUDED_CONDUCTING_ITEM_H

#include "Item.h"

class ConductingItem : public Item
{
public:

	ConductingItem(const std::string &itemName, const std::string &inventoryTextureName, const std::string &textureName, int id);

	virtual void onUse(Game &game) final;

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	// If the item is used with the proper safety gear
	virtual void onSafeUsage(Game &game) {};

protected:

	bool m_conducting;

};

#endif