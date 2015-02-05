#include "KeyItem.h"


KeyItem::KeyItem(const std::string &textureName, int id)
:
Item("Key", textureName, id)
{
	m_lootable = true;
}

bool KeyItem::onInteract(Item &otherItem)
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