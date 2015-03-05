#include "CableItem.h"

CableItem::CableItem(int id)
:
Item("Cable","CablesInv", "CableItem", id)
{
	m_lootable = true;
	m_examineString = "A worthless piece of fucking cable.";//"It's an electrical cable, it appears to be in working condition.";
	m_useString = "I can't do much with just the cable";
}

bool CableItem::onInteract(Item &otherItem, Game &game)
{
	// TODO add eletrical items for which it can be put in
	return false;
}

Item* CableItem::clone()
{
	return new CableItem(*this);
}