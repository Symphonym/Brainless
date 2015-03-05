#include "NewspaperItem.h"

NewspaperItem::NewspaperItem(int id)
:
Item("Newspaper", "NewspaperItem", "SmallNewspaperItem", id)
{
	m_lootable = true;
	m_usable = false;
	m_useString = "I would try to read this, but it appears to be written in ancient l33t sp34k";
	m_examineString = "It's a newspaper, it appears to be from the year 1337";
}

Item* NewspaperItem::clone()
{
	return new NewspaperItem(*this);
}