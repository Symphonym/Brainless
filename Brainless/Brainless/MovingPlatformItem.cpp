#include "MovingPlatformItem.h"
#include "Renderer.h"

MovingPlatformItem::MovingPlatformItem(sf::Vector2f speed, float maxDistance, int id)
:
Item("MovingPlatform", "MovingPlatform", id),
m_speed(speed),
m_maxDistanceMoved(maxDistance)
{
	m_collidable = true;
	m_solid = false;

	m_collisionSize = sf::Vector2f(250, 50);
	m_collisionOffset = sf::Vector2f(0, 0);
}


void MovingPlatformItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);

	writer << m_speed.x << std::endl;
	writer << m_speed.y << std::endl;
	writer << m_distanceMoved.x << std::endl;
	writer << m_distanceMoved.x << std::endl;
}
void MovingPlatformItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);

	reader >> m_speed.x >> m_speed.y;
	reader >> m_distanceMoved.x >> m_distanceMoved.y;
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

sf::Vector2f MovingPlatformItem::getSpeed() const
{
	return m_speed;
}
