#ifndef INCLUDED_ROPE_LADDER_ITEM_H
#define INCLUDED_ROPE_LADDER_ITEM_H

#include "Item.h"
#include "LadderItem.h"

class RopeLadderItem : public Item
{
public:

	RopeLadderItem(const std::string &ladderTexture, int id);

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	virtual void onUse(Game &game);
	virtual void update(float deltaTime, Game &game);
	virtual void draw();

	virtual void onPositionChanged();
	virtual bool onInteractedWith(Item &otherItem, Game &game);

	virtual Item* clone();

private:

	LadderItem m_ladder;
	bool m_ladderIsDown;


	void refreshStatus();
};

#endif