#include "BrainItem.h"

BrainItem::BrainItem(int id)
:
Item("Brain", "BrainItem", id)
{
	m_lootable = true;
	m_examineString = "It's a brain, most likely belonging to one of the zombies";
	m_useString = "I don't think I should do that";
}

bool BrainItem::onInteractUnit(Item &otherItem, Game &game)
{
	return false;
}

Item* BrainItem::clone()
{
	return new BrainItem(*this);
}