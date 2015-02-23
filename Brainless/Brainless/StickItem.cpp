#include "StickItem.h"

StickItem::StickItem(int id)
:
Item("Stick","StickInv", "StickItem", id)
{
	m_lootable = true;
	m_examineString = "It's a pretty short stick";
	m_useString = "That's not going to work";
}

Item* StickItem::clone()
{
	return new StickItem(*this);
}