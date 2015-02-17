#include "SpiritFeatherItem.h"
#include "Unit.h"
#include "Game.h"
#include "Renderer.h"


SpiritFeatherItem::SpiritFeatherItem(int id)
:
Item("Spirit Feather", "SpiritFeather", id),
m_animation(100,100)
{
	m_collisionSize = sf::Vector2f(40, 100);
	m_collisionOffset = sf::Vector2f(30, 0);
	m_examineString = "It's a feather that's mystically hovering around";
	m_useString = "I don't think I should do that";
	m_animation.loop(0, 7, 0, 10);
	getSprite().setTextureRect(m_animation.getRectangle(0));
}

bool SpiritFeatherItem::onCollisionWithUnit(Unit &unit, Game &game)
{
	// Delete item on player collision and add spirit power
	if (unit.getUnitType() == Unit::ID_Player)
	{
		game.getSpiritBar().addValue(Constants::SpiritFeatherWorth);
		return true;
	}
	else
		return false;
}

void SpiritFeatherItem::update(float deltaTime, Game &game)
{
	getSprite().setTextureRect(m_animation.getRectangle(deltaTime));
}

void SpiritFeatherItem::draw()
{
	// draw animation with getSprite()
	Renderer::instance().drawAbove(getSprite());
}

Item* SpiritFeatherItem::clone()
{
	return new SpiritFeatherItem(*this);
}