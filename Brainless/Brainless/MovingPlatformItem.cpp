#include "MovingPlatformItem.h"
#include "Renderer.h"

MovingPlatformItem::MovingPlatformItem(sf::Vector2f speed, float maxDistance, int id)
:
Item("MovingPlatform","MovingPlatform", "MovingPlatform", id),
m_speed(speed),
m_maxDistanceMoved(maxDistance),
m_isActive(true)
{
	m_collidable = true;
	m_solid = false;
	m_examineString = "MAGIC? What is this… it's moving?! What is this S*per M*ario? ";

	m_collisionBounds = sf::FloatRect(0, 0, 250, 50);
}


void MovingPlatformItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);

	writer << m_speed.x << std::endl;
	writer << m_speed.y << std::endl;
	writer << m_distanceMoved.x << std::endl;
	writer << m_distanceMoved.y << std::endl;
	writer << m_isActive << std::endl;
}
void MovingPlatformItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);

	reader >> m_speed.x >> m_speed.y;
	reader >> m_distanceMoved.x >> m_distanceMoved.y;
	reader >> m_isActive;
}

bool MovingPlatformItem::onSyncedWith(Item &otherItem)
{
	m_isActive = !m_isActive;
	return false;
}

void MovingPlatformItem::update(float deltaTime, Game &game)
{
	if (m_isActive)
	{
		sf::Vector2f speed = m_speed * deltaTime;
		setPosition(sf::Vector2f(getPosition().x + speed.x, getPosition().y + speed.y));
		m_distanceMoved += sf::Vector2f(abs(speed.x), abs(speed.y));

		if (m_distanceMoved.x > m_maxDistanceMoved || m_distanceMoved.y > m_maxDistanceMoved)
		{
			m_distanceMoved = sf::Vector2f(0, 0);
			m_speed = -m_speed;
		}
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
	if (m_isActive)
		return m_speed;
	else
		return sf::Vector2f(0, 0);
}
