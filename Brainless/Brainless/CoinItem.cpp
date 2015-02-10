#include "CoinItem.h"

CoinItem::CoinItem(int id)
:
Item("Coin", "CoinItem", id)
{
	m_lootable = true;

	addCombination(4, 11, true);
}

bool CoinItem::onInteract(Item &otherItem)
{
	if (otherItem.getName() == "Vending Machine")
		return true;
	else
		return false;
}

Item* CoinItem::clone()
{
	return new CoinItem(*this);
}