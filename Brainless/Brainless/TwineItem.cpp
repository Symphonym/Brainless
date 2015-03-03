#include "TwineItem.h"

TwineItem::TwineItem(int id)
:
Item("Twine","TwineInv", "TwineItem", id)
{
	m_lootable = true;
	m_examineString = "An ordinary piece of twine.";
}

Item* TwineItem::clone()
{
	return new TwineItem(*this);
}