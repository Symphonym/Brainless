#include "StoneItem.h"

StoneItem::StoneItem(int id)
:
Item("Stone","StoneInv", "StoneItem", id)
{
	m_lootable = true;
	m_examineString = "It's a stone, usable for smashing things";
}

Item* StoneItem::clone()
{
	return new StoneItem(*this);
}