#include "StickItem.h"

StickItem::StickItem(int id)
:
Item("Stick","StickInv", "StickItem", id)
{
	m_lootable = true;
	m_examineString = "Well it's a… stick. Good if I ever got into a sticky situation.";
	m_useString = "That's not going to work";
}

Item* StickItem::clone()
{
	return new StickItem(*this);
}