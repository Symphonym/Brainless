#ifndef INCLUDED_SPIRIT_FEATHER_ITEM_H
#define INCLUDED_SPIRIT_FEATHER_ITEM_H

#include "Item.h"
#include "Animation.h"

class SpiritFeatherItem : public Item
{
public:

	SpiritFeatherItem(int id);

	virtual bool onCollisionWithUnit(Unit &unit, Game &game);
	virtual void update(float deltaTime, Game &game);
	virtual void draw();

	virtual Item* clone();

private:
	int m_currentFrame = 0;
	int m_maxFrame = 8;
	Animation m_animation;

};

#endif