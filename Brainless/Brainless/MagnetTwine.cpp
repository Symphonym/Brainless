#include "MagnetTwine.h"

MagnetTwine::MagnetTwine(int id)
:
Item("Magnet & Twine", "MagnetTwineInv", "MagnetTwineInv", id)
{
	m_lootable = true;
	m_examineString = "It's a magnet attached to a string";
}

Item* MagnetTwine::clone()
{
	return new MagnetTwine(*this);
}