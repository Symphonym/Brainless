#include "NewspaperItem.h"

NewspaperItem::NewspaperItem(int id)
:
Item("Newspaper", "NewspaperItem", "SmallNewspaperItem", id)
{
	m_lootable = true;
	m_usable = false;
	m_useString = "I would try to read this, but it appears to be written in ancient l33t sp34k";
	m_examineString = "\"The mayor neglected to put out the fire caused by his pipe and I don't care very much I quit, Joe I had an affair with your bayfriend, have fun with that.\" That makes sense. Maybe?";
}

Item* NewspaperItem::clone()
{
	return new NewspaperItem(*this);
}