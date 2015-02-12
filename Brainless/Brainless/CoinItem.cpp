#include "CoinItem.h"

CoinItem::CoinItem(int id)
:
Item("Coin", "CoinItem", id)
{
	m_lootable = true;

	addCombination(4, 11, true);
}

bool CoinItem::onInteract(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Vending Machine")
	{
		SoundPlayer::instance().playSound("item_coin", getPosition());
		return true;
	}
	else
		return false;
}

Item* CoinItem::clone()
{
	return new CoinItem(*this);
}