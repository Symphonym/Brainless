#include "DoubleStickItem.h"



DoubleStickItem::DoubleStickItem(int id)
:
Item("Two Sticks", "DoubleStickItem", id)
{
	m_lootable = true;
	m_examineString = "It's two sticks";
	addCombination(0, 19, true);
}

Item* DoubleStickItem::clone()
{
	return new DoubleStickItem(*this);
}