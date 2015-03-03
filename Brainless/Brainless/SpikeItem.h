#ifndef INCLUDED_SPIKE_ITEM_H
#define INCLUDED_SPIKE_ITEM_H

#include "Item.h"

class SpikeItem : public Item
{
public:

	SpikeItem(int id);

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	virtual bool onInteractedWith(Item &otherItem, Game &game);
	virtual bool onCollisionWithUnit(Unit &unit, Game &game);

	bool isDampened() const;

	virtual Item* clone();

private:

	bool m_spikesDampened;
	void refreshTexture();
};

#endif