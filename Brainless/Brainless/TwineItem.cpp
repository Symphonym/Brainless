#include "TwineItem.h"

TwineItem::TwineItem(int id)
:
Item("Twine","TwineInv", "TwineItem", id)
{
	m_lootable = true;
}

Item* TwineItem::clone()
{
	return new TwineItem(*this);
}