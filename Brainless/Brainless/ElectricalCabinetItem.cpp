#include "ElectricalCabinetItem.h"
#include "ResourceLoader.h"
#include "Renderer.h"

ElectricalCabinetItem::ElectricalCabinetItem(int id)
:
Item("Cabinet", "electricalCabinet", "electricalCabinet", id)
{
	m_usable = true;
	m_collidable = true;

	m_collisionBounds = sf::FloatRect(46,62,190,286);
}

void ElectricalCabinetItem::onExamine()
{
	m_examineString = "Oh shiny, maybe I should touch it";
}

void ElectricalCabinetItem::draw()
{
	Renderer::instance().drawDepth(getSprite());
}

Item* ElectricalCabinetItem::clone()
{
	return new ElectricalCabinetItem(*this);
}
