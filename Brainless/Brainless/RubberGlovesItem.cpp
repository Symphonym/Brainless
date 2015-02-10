#include "RubberGlovesItem.h"

RubberGlovesItem::RubberGlovesItem(int id)
:
Item("Rubber Gloves", "RubberGlovesItem", id)
{
	m_lootable = true;

	m_examineString = "It's a pair of rubber gloves";
	m_usable = "That's not going to work";
}

Item* RubberGlovesItem::clone()
{
	return new RubberGlovesItem(*this);
}