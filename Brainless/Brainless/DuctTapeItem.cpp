#include "DuctTapeItem.h"


DuctTapeItem::DuctTapeItem(int id)
:
Item("Duct Tape","DuctTapeInv", "DuctTapeItem", id)
{
	m_lootable = true;

	m_examineString = "Duct tape can solve any problem in the universe! Except maths.";
	m_useString = "Not sure I can use it that way.";
}


Item* DuctTapeItem::clone()
{
	return new DuctTapeItem(*this);
}