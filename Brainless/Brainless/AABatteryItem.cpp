#include "AABatteryItem.h"


AABatteryItem::AABatteryItem(int id)
:
Item("AA Battery","AABatteriesInv", "AABatteryItem", id)
{
	m_lootable = true;
	m_examineString = "A battery. Let's see if I can power something with this!";
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