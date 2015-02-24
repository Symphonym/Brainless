#include "ManholeItem.h"


ManholeItem::ManholeItem(bool isOpen, int id)
:
Item("Manhole", "ManholeClosed", "ManholeClosed", id),
m_open(isOpen)
{
	m_lootable = false;
	m_usable = true;
	m_solid = false;
	m_collidable = true;
	m_examineString = "It reminds Maive of her favorite tv-show. Teenage Mutant Ninja Åke!";
}

void ManholeItem::onUse(Game &game)
{
	m_open = !m_open;
}

Item* ManholeItem::clone()
{
	return new ManholeItem(*this);
}