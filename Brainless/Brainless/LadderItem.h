#ifndef INCLUDED_LADDER_ITEM_H
#define INCLUDED_LADDER_ITEM_H

#include "Item.h"

class LadderItem : public Item
{
public:

	LadderItem(const std::string &ladderTexture, int id);


	virtual void onUse(Game &game);
	virtual void update(float deltaTime, Game &game);

	virtual Item* clone();

private:

	// Whether or not the ladder is in use, it's not usable if it is
	bool m_inUse;
	bool m_reachedStartPos;

	float m_lerpValue;

	// Start position when end is the original start pos
	sf::Vector2f m_startStartPos;

	sf::Vector2f m_startPos;
	sf::Vector2f m_endPos;

};

#endif