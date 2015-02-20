#include "LongStickItem.h"

LongStickItem::LongStickItem(int id)
:
Item("Long Stick","LongStickInv", "LongStickItem", id)
{
	m_lootable = true;
	m_examineString = "It's two sticks duct taped together";
}

Item* LongStickItem::clone()
{
	return new LongStickItem(*this);
}