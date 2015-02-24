#ifndef INCLUDED_ARCADE_MACHINE_ITEM_H
#define INCLUDED_ARCADE_MACHINE_ITEM_H

#include "Item.h"
#include <vector>

class ArcadeMachineItem : public Item
{
public:

	ArcadeMachineItem(int id);

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	virtual void onUse(Game &game);
	virtual bool onInteractedWith(Item &otherItem, Game &game);
	virtual Item* clone();

private:

	std::vector<int> m_loadedGameIDs;

};

#endif