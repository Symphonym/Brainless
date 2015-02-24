#ifndef INCLUDED_MANHOLE_ITEM_H
#define INCLUDED_MANHOLE_ITEM_H

#include "Item.h"
#include "LadderItem.h"

class ManholeItem : public Item
{
public:

	ManholeItem(bool isOpen, const std::string &ladderTexture, int id);

	virtual void onUse(Game &game);
	virtual void onPositionChanged();

	virtual void update(float deltaTime, Game &game);
	virtual void draw();

	virtual Item* clone();

private:
	
	LadderItem m_ladder;
	bool m_open;


	void refreshTexture();
};

#endif