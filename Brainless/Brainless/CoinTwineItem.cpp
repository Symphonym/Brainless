#include "CoinTwineItem.h"


CoinTwineItem::CoinTwineItem(int id)
:
Item("String & Coin", "CoinTwineItem", id)
{
	m_lootable = true;
}

bool CoinTwineItem::onInteract(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Vending Machine")
		return false;
	else
		return false; // Idk if other logic here
}

Item* CoinTwineItem::clone()
{
	return new CoinTwineItem(*this);
}

