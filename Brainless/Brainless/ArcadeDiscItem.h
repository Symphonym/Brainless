#ifndef INCLUDED_ARCADE_DISC_ITEM_H
#define INCLUDED_ARCADE_DISC_ITEM_H

#include "Item.h"
#include <functional>

class ArcadeGame;
class ArcadeMachine;
class ArcadeDiscItem : public Item
{
public:
	typedef std::function<ArcadeGame*(ArcadeMachine &machine)> GameCreatorFunc;

	ArcadeDiscItem(int id, const std::string &colorName, GameCreatorFunc gameCreator);

	ArcadeGame* createArcadeGame(ArcadeMachine &machine);

	virtual bool onInteract(Item &otherItem, Game &game);
	virtual Item* clone();

private:

	GameCreatorFunc m_gameCreatorFunc;
};

#endif