#include "ArcadeDiscItem.h"

ArcadeDiscItem::ArcadeDiscItem(int id, const std::string &colorName, ArcadeDiscItem::GameCreatorFunc gameCreator)
:
Item("Arcade Game","ArcadeDiscItem" + colorName + "Big", "ArcadeDiscItem" + colorName + "Small", id),
m_gameCreatorFunc(gameCreator)
{
	m_lootable = true;
	m_examineString = "Retro! I miss the arcade. And games. And witty commentary.";
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
