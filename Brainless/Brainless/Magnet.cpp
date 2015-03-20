#include "Magnet.h"


Magnet::Magnet(int id)
:
Item("Magnet", "MagnetInv", "MagnetItem", id)
{
	m_examineString = "It's a magnet, with it you can do everything?";
	m_lootable = true;
}

Item* Magnet::clone()
{
	return new Magnet(*this);
}