#include "SpiritFeatherSpawnItem.h"
#include "Game.h"
#include "ItemDatabase.h"

SpiritFeatherSpawnItem::SpiritFeatherSpawnItem(int id)
:
Item("Spirit Feather Spawner", "ChestOpen", "ChestOpen", id)
{
	m_usable = false;
	m_lootable = false;
	m_examineString = "It has a magical aura around it";
	m_useString = "I don't think I should do that";
}

void SpiritFeatherSpawnItem::update(float deltaTime, Game &game)
{
	int featherCount = 0;
	for (auto &item : game.getLevel().getItems())
	{
		if (item->getName() == "Spirit Feather")
			++featherCount;
	}

	if (featherCount == 0 && game.getSpiritBar().getValue() < game.getSpiritBar().getMaxValue())
	{
		ItemDatabase::ItemPtr item = ItemDatabase::instance().extractItem(28);
		item->setPosition(sf::Vector2f(
			getPosition().x + getSprite().getGlobalBounds().width / 2.f,
			getPosition().y + getSprite().getGlobalBounds().height / 2.f));

		if (item)
			game.getLevel().addItem(std::move(item));
	}
}

Item* SpiritFeatherSpawnItem::clone()
{
	return new SpiritFeatherSpawnItem(*this);
}