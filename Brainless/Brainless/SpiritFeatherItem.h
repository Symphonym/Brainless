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

	void flash();

private:
	int m_currentFrame = 0;
	Animation m_animation;
	
	float m_flashAlpha = 0;
};

#endif