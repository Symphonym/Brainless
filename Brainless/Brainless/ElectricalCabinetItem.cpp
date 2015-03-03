#include "ElectricalCabinetItem.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "ItemDatabase.h"
#include "Unit.h"
#include "Game.h"
#include <iostream>

ElectricalCabinetItem::ElectricalCabinetItem(std::vector<int> itemsWithin, int id)
:
Item("Cabinet", "electricalCabinet", "electricalCabinet", id),
m_itemsWithin(itemsWithin)
{
	m_usable = true;
	m_collidable = true;

	m_collisionBounds = sf::FloatRect(46,62,190,286);
}

void ElectricalCabinetItem::onExamine()
{
	m_examineString = "There's a cable loose, but it's still working.";
}

void ElectricalCabinetItem::draw()
{
	Renderer::instance().drawDepth(getSprite());
}

Item* ElectricalCabinetItem::clone()
{
	return new ElectricalCabinetItem(*this);
}
int ElectricalCabinetItem::getScriptUnitType()
{
	return Unit::UnitType::ID_WalkingZombie;
}

bool ElectricalCabinetItem::onCollisionWithUnit(Unit &unit, Game &game)
{
	//test
	if (unit.getUnitType() == Unit::UnitType::ID_WalkingZombie)
	{
		sf::Vector2f floorPosition(getPosition().x + getSprite().getGlobalBounds().width/2, getPosition().y + getSprite().getGlobalBounds().height/2);

		// Spawn items when chest is opened
		for (std::size_t i = 0; i < m_itemsWithin.size(); i++)
		{


			ItemDatabase::ItemPtr item = std::move(ItemDatabase::instance().extractItem(m_itemsWithin[i]));

			// Make sure items in the chest are lootable
			if (!item->isLootable())
				continue;

			item->setPosition(sf::Vector2f(floorPosition.x, floorPosition.y - item->getSprite().getGlobalBounds().height));

			int offset = std::rand() % 150;
			if (std::rand() % 2 == 0)
				offset *= -1;

			item->setPosition(sf::Vector2f(item->getPosition().x + offset, item->getPosition().y));

			game.getLevel().addItemToBuffer(std::move(item));
		}
		m_itemsWithin.clear();

		flyOff();

	}

	return false;
}