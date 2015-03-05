#include "CoinItem.h"

CoinItem::CoinItem(int id)
:
Item("Coin","CoinsInv", "CoinItem", id)
{
	m_examineString = "Coins lying around, what is this, Mario?";
	m_lootable = true;
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