#include "EmptyBagItem.h"


EmptyBagItem::EmptyBagItem(int id)
:
Item("Empty Bag", "EmptyBagItem", "EmptyBagItem", id)
{
	m_usable = false;
	m_lootable = true;
	m_examineString = "It's an empty bag, maybe I can put something in it";
	m_useString = "I guess I could wave it in the air but that would be silly";
}

Item* EmptyBagItem::clone()
{
	return new EmptyBagItem(*this);
}