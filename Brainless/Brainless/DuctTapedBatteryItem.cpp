#include "DuctTapedBatteryItem.h"

DuctTapedBatteryItem::DuctTapedBatteryItem(int id)
:
Item("AA Battery & Duct Tape", "DuctTapeBatteryItem", id)
{
	m_lootable = true;
	m_examineString = "It's an AA battery with duct tape. It's ready to be attached to things.";
	addCombination(25, 26, true);
}

Item* DuctTapedBatteryItem::clone()
{
	return new DuctTapedBatteryItem(*this);
}