#include "SpiritFeatherItem.h"
#include "Unit.h"

SpiritFeatherItem::SpiritFeatherItem(int id)
:
Item("Spirit Feather", "KEVIN_INSERT_SPRITESHIT_HERE", id)
{

	m_examineString = "It's a feather that's mystically hovering around";
	m_useString = "I don't think I should do that";
}

bool SpiritFeatherItem::onCollisionWithUnit(Unit &unit, Game &game)
{
	// Delete item on player collision
	if (unit.getUnitType() == Unit::ID_Player)
	{
		return true;
	}
	else
		return false;
}
void SpiritFeatherItem::draw()
{
	// draw animation with getSprite()
}

Item* SpiritFeatherItem::clone()
{
	return new SpiritFeatherItem(*this);
}