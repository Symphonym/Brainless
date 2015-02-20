#include "HammerItem.h"

HammerItem::HammerItem(int id)
:
Item("Hammer","HammerInv", "HammerItem", id)
{
	m_lootable = true;
	m_examineString = "It's a hammer, the best tool of them all";
	m_useString = "There's nothing to use a hammer on";
	addCombination(24, 26, true);
}

bool HammerItem::onInteract(Item &otherItem, Game &game)
{
	// TODO add hammer interactions
	return false;
}

Item* HammerItem::clone()
{
	return new HammerItem(*this);
}