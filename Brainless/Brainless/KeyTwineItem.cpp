#include "KeyTwineItem.h"

KeyTwineItem::KeyTwineItem(int id)
:
Item("Key & String", "KeyTwineItem", id)
{
	m_lootable = true;
	m_examineString = "It's a key attached to a string";
}

Item* KeyTwineItem::clone()
{
	return new KeyTwineItem(*this);
}