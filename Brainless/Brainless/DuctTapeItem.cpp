#include "DuctTapeItem.h"


DuctTapeItem::DuctTapeItem(int id)
:
Item("Duct Tape","DuctTapeItem", "DuctTapeItem", id)
{
	m_lootable = true;

	m_examineString = "It's a roll of duct tape, usefull for everything.";
	m_useString = "Not sure I can use it that way.";
	// TODO ADD COMBINATIONS

	addCombination(18, 19, true);
	addCombination(22, 24, true);
}


Item* DuctTapeItem::clone()
{
	return new DuctTapeItem(*this);
}