#include "CarBatteryItem.h"


CarBatteryItem::CarBatteryItem(int id)
:
Item("Car Battery","CarBatteryInv", "CarBatteryItem", id)
{
	m_lootable = true;
	m_examineString = "A battery. Let's see if I can power something with this!";
	m_useString = "There's nothing here to use with a car battery";
}

bool CarBatteryItem::onInteractedWith(Item &otherItem, Game &game)
{
	// TODO add battery interactions with electrical stuff
	return false;
}

Item* CarBatteryItem::clone()
{
	return new CarBatteryItem(*this);
}