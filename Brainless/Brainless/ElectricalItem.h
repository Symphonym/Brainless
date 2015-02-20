#ifndef INCLUDED_ELECTRICAL_ITEM_H
#define INCLUDED_ELECTRICAL_ITEM_H

#include "Item.h"

class EletricalItem : public Item
{
public:

	enum BatteryTypes
	{
		AA = 0,
		Car = 1
	};

	EletricalItem(const std::string &itemName, const std::string &inventoryTextureName, const std::string &textureName, int id);

	virtual void onUse(Game &game) final;
	virtual void onUseOn(Game &game) {};
	virtual void onUseOff(Game &game) {};

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	virtual bool onInteractedWith(Item &otherItem, Game &game);

protected:

	// If the item is lacking any batteries
	BatteryTypes m_batteryType;
	int m_missingBatteries;

	bool m_hasEletricity;
};

#endif