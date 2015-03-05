#include "TwineItem.h"

TwineItem::TwineItem(int id)
:
Item("Twine","TwineInv", "TwineItem", id)
{
	m_lootable = true;
	m_examineString = "This might just be the most useful thing ever! Or twine.";
}

Item* TwineItem::clone()
{
	return new TwineItem(*this);
}