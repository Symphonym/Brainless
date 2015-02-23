#include "AABatteryItem.h"


AABatteryItem::AABatteryItem(int id)
:
Item("AA Battery","AABatteriesInv", "AABatteryItem", id)
{
	m_lootable = true;
	m_examineString = "It's an AA battery. It appears to have some power left";
	m_useString = "Just using a battery on it's own isn't going to do much";
}

bool AABatteryItem::onInteract(Item &otherItem, Game &game)
{
	// TODO add interaction with electrical stuff

	return false;
}

Item* AABatteryItem::clone()
{
	return new AABatteryItem(*this);
}