#include "StickItem.h"

StickItem::StickItem(int id)
:
Item("Stick","StickItem", "StickItem", id)
{
	m_lootable = true;
	m_examineString = "It's a pretty short stick";
	m_useString = "That's not going to work";

	addCombination(17, 18, true);
}

Item* StickItem::clone()
{
	return new StickItem(*this);
}