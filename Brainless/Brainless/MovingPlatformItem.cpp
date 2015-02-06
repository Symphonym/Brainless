#include "MovingPlatformItem.h"
#include "Renderer.h"

MovingPlatformItem::MovingPlatformItem(sf::Vector2f speed, float maxDistance, int id)
:
Item("MovingPlatform", "MovingPlatform", id),
m_speed(speed),
m_maxDistanceMoved(maxDistance)
{
	m_collidable = true;

	m_collisionSize = sf::Vector2f(250, 50);
	m_collisionOffset = sf::Vector2f(0, 0);
}

void MovingPlatformItem::update(float deltaTime, Game &game)
{
	setPosition(sf::Vector2f(getPosition().x + m_speed.x, getPosition().y + m_speed.y));
	m_distanceMoved += sf::Vector2f(abs(m_speed.x), abs(m_speed.y));

	if (m_distanceMoved.x > m_maxDistanceMoved || m_distanceMoved.y > m_maxDistanceMoved)
	{
		m_distanceMoved = sf::Vector2f(0, 0);
		m_speed = -m_speed;
	}
}

void MovingPlatformItem::onExamine()
{
	m_examineString = "This platform is moving by the power of love.";
}



void MovingPlatformItem::draw()
{
	Renderer::instance().drawAbove(getSprite());
}

Item* MovingPlatformItem::clone()
{
	return new MovingPlatformItem(*this);
}
