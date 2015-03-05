#include "HammerItem.h"

HammerItem::HammerItem(int id)
:
Item("Hammer","HammerInv", "HammerItem", id)
{
	m_lootable = true;
	m_examineString = "Let's get hammered!";
	m_useString = "There's nothing to use a hammer on";
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