#ifndef INCLUDED_GHOST_ITEM_H
#define INCLUDED_GHOST_ITEM_H

#include "Item.h"
#include "DialogTree.h"
#include "Animation.h"

#include <string>

class GhostItem : public Item
{
public:

	explicit GhostItem(int id,int ghost_id);

	virtual void update(float deltaTime, Game &game);
	virtual void draw();

	virtual void onUse(Game &game);
	virtual bool onCollisionWithUnit(Unit &unit, Game &game);

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	virtual Item* clone();

private:
	float m_currentFrame = 0;
	int m_frameCount = 13;
	Animation m_animation;

	// Dialog of the ghost
	int m_ghostID;
	DialogTree m_dialog;
};

#endif