#ifndef INCLUDED_PUDDLE_CABLE_ITEM_H
#define INCLUDED_PUDDLE_CABLE_ITEM_H

#include "Item.h"
#include "Animation.h"

class PuddleCableItem : public Item
{
public:

	PuddleCableItem(bool cableInPuddle, int id);

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);
	virtual void draw();

	virtual bool onCollisionWithUnit(Unit &unit, Game &game);
	virtual bool onInteractedWith(Item &otherItem, Game &game);
	bool isActive();
	virtual void update(float deltaTime, Game &game);

	virtual Item* clone();

private:

	int m_currentFrame = 0;
	int m_maxFrame = 4;
	Animation m_animation;
	sf::Sprite m_electricitySprite;

	float m_particleTime = 0;
	void refreshStatus();
	bool m_cableInPuddle;
};

#endif