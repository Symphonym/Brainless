#include "TwineItem.h"

TwineItem::TwineItem(int id)
:
Item("Twine","TwineInv", "TwineItem", id)
{
	m_lootable = true;
	addCombination(3, 11, true);
	addCombination(7, 13, true);
}

Item* TwineItem::clone()
{
	return new TwineItem(*this);
}