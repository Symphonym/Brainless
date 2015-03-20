#include "RubberGlovesItem.h"

RubberGlovesItem::RubberGlovesItem(int id)
:
Item("Rubber Gloves","RubberGloveInv", "RubberGlovesItem", id)
{
	m_lootable = true;

	m_examineString = "What a pretty color! I would prefer rubber duckies though.";
	m_usable = "That's not going to work";
}

Item* RubberGlovesItem::clone()
{
	return new RubberGlovesItem(*this);
}

bool RubberGlovesItem::onInteract(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Eletrical Puddle")
	{
		if (otherItem.isActive())
		return true;
	}
	else
		return false;
}