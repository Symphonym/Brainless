#include "BrainItem.h"
#include "Unit.h"

BrainItem::BrainItem(int id)
:
Item("Brain","BrainItem", "BrainItem", id)
{
	m_lootable = true;
	m_examineString = "Someones brain, that's kind of nasty.";
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