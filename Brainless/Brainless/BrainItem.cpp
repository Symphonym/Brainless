#include "BrainItem.h"
#include "Unit.h"

BrainItem::BrainItem(int id)
:
Item("Brain","BrainItemInv", "BrainItem", id)
{
	m_lootable = true;
	m_examineString = "I wonder what sound it would make if I slammed it back into someones head.";
	m_useString = "I don't think I should do that";
}

bool BrainItem::onInteractUnit(Unit &unit)
{

	return unit.getUnitType() != unit.ID_Player
		&& unit.getUnitType() != unit.ID_CabinetZombie 
		&& unit.getUnitType() != unit.ID_ItemZombie
		&& unit.getUnitType() != unit.ID_NONE;
}

Item* BrainItem::clone()
{
	return new BrainItem(*this);
}