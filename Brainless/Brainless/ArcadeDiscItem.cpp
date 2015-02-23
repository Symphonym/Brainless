#include "ArcadeDiscItem.h"

ArcadeDiscItem::ArcadeDiscItem(int id, ArcadeDiscItem::GameCreatorFunc gameCreator)
:
Item("Arcade Game", "ArcadeDiscItem", "ArcadeDiscItem", id),
m_gameCreatorFunc(gameCreator)
{
	m_lootable = true;
	m_examineString = "It's an arcade game, it might work with an arcade machine";
	m_useString = "I'm need an arcade machine to do anything with it";
}

ArcadeGame* ArcadeDiscItem::createArcadeGame(ArcadeMachine &machine)
{
	return m_gameCreatorFunc(machine);
}

bool ArcadeDiscItem::onInteract(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Arcade Machine")
		return true;
	else
		return false;
}

Item* ArcadeDiscItem::clone()
{
	return new ArcadeDiscItem(*this);
}
