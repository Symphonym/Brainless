#include "StoneItem.h"

StoneItem::StoneItem(int id)
:
Item("Stone","StoneInv", "StoneItem", id)
{
	m_lootable = true;
	m_examineString = "An ordinary stone, makes me want to smash stuff.";
}

Item* StoneItem::clone()
{
	return new StoneItem(*this);
}