#include "ElectricalCabinetItem.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "ItemDatabase.h"
#include "Unit.h"
#include "Game.h"
#include <iostream>
#include "SoundPlayer.h"

ElectricalCabinetItem::ElectricalCabinetItem(std::vector<int> itemsWithin, int id)
:
Item("Cabinet", "electricalCabinet", "electricalCabinet", id),
m_itemsWithin(itemsWithin)
{
	m_usable = false;
	m_collidable = true;

	m_collisionBounds = sf::FloatRect(46,62,190,286);
	m_interactBounds = m_collisionBounds;
}

void ElectricalCabinetItem::onExamine()
{
	m_examineString = "A mystery from a time long gone, no one ever knew what these were good for.";
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
	return Unit::UnitType::ID_CabinetZombie;
}

bool ElectricalCabinetItem::onCollisionWithUnit(Unit &unit, Game &game)
{

	if (unit.getUnitType() == Unit::UnitType::ID_CabinetZombie)
	{

		SoundPlayer::instance().playSound("cabinetFlyOff", game.getWindow().getView().getCenter());

		flyOff();
	}

	return false;
}