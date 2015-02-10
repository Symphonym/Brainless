#include "DuctTapeItem.h"


DuctTapeItem::DuctTapeItem(int id)
:
Item("Duct Tape", "DuctTapeItem", id)
{
	m_lootable = true;

	m_examineString = "It's a roll of duct tape, usefull for everything.";
	m_useString = "Not sure I can use it that way.";
	// TODO ADD COMBINATIONS
}


Item* DuctTapeItem::clone()
{
	return new DuctTapeItem(*this);
}