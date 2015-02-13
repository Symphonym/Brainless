#ifndef INCLUDED_ELECTRICAL_ITEM_H
#define INCLUDED_ELECTRICAL_ITEM_H

#include "Item.h"

class EletricalItem : public Item
{
public:

	EletricalItem(const std::string &itemName, const std::string &textureName, int id);

	virtual void onUse(Game &game) final;
	virtual void onUseOn(Game &game) {};
	virtual void onUseOff(Game &game) {};

	virtual bool onInteractedWith(Item &otherItem, Game &game);

protected:

	bool m_hasEletricity;
};

#endif