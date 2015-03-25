#include "StoneItem.h"

StoneItem::StoneItem(int id)
:
Item("Stone","StoneInv", "StoneItem", id)
{
	m_lootable = true;
	m_examineString = "The possibilities. Like, smashing stuff. Or throwing it. Or throwing it at stuff to smash. Or I could keep her, Leslie, the pet rock. Let's rock!.";
}

Item* StoneItem::clone()
{
	return new StoneItem(*this);
}

bool StoneItem::onInteract(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Window")
	{
		if (otherItem.isActive())
			return true;
	}
	else if (otherItem.getName() == "WindowOverlay")
	{
		return true;
	}
	return false;
}