#include "KeyItem.h"


KeyItem::KeyItem(int id)
:
Item("Key","KeyInv", "KeyTest", id)
{
	m_lootable = true;
}

bool KeyItem::onInteract(Item &otherItem, Game &game)
{
	// A key is consumed if it can be used
	if (otherItem.getSyncID() == getSyncID())
		return true;
	else
		return false;
}

Item* KeyItem::clone()
{
	return new KeyItem(*this);
}