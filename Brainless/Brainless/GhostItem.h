#ifndef INCLUDED_GHOST_ITEM_H
#define INCLUDED_GHOST_ITEM_H

#include "Item.h"
#include "DialogTree.h"
#include "Animation.h"

#include <string>

class GhostItem : public Item
{
public:

	explicit GhostItem(const std::string &dialogFile, int id);

	virtual void update(float deltaTime, Game &game);
	virtual void draw();

	virtual void onUse(Game &game);

	virtual Item* clone();

private:
	float m_currentFrame = 0;
	int m_frameCount = 13;

	// Dialog of the ghost
	DialogTree m_dialog;
};

#endif