#include "BrainItem.h"
#include "Unit.h"

BrainItem::BrainItem(int id)
:
Item("Brain","BrainItem", "BrainItem", id)
{
	m_lootable = true;
	m_examineString = "It's a brain, most likely belonging to one of the zombies";
	m_useString = "I don't think I should do that";
}

bool BrainItem::onInteractUnit(Unit &unit)
{
	return unit.getUnitType() != unit.ID_Player;
}

Item* BrainItem::clone()
{
	return new BrainItem(*this);
}