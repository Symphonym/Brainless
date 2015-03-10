#ifndef INCLUDED_PUDDLE_CABLE_ITEM_H
#define INCLUDED_PUDDLE_CABLE_ITEM_H

#include "Item.h"

class PuddleCableItem : public Item
{
public:

	PuddleCableItem(bool cableInPuddle, int id);

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	virtual bool onCollisionWithUnit(Unit &unit, Game &game);
	virtual bool onInteractedWith(Item &otherItem, Game &game);

	virtual void update(float deltaTime, Game &game);

	virtual Item* clone();

private:
	float m_particleTime = 0;
	void refreshStatus();
	bool m_cableInPuddle;
};

#endif