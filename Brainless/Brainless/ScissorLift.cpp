#include "ScissorLift.h"
#include "Renderer.h"
#include "ResourceLoader.h"

ScissorLiftItem::ScissorLiftItem(sf::Vector2f speed, float maxDistance, int id)
:
Item("ScissorLift","liftBottom", "liftBottom", id),
m_speed(speed),
m_maxDistanceMoved(maxDistance),
m_isActive(true),
m_maxDistance(maxDistance),
m_platform(speed,maxDistance,id)
{
	m_collidable = true;
	m_solid = false;
	m_lootable = false;

	m_collisionBounds = sf::FloatRect(0, 0, 0, 0);
	m_xTexture = sf::Sprite(ResourceLoader::instance().retrieveTexture("liftX"),
		sf::IntRect(0, 0, 0, 0));
	m_topTexture = sf::Sprite(ResourceLoader::instance().retrieveTexture("liftTop"),
		sf::IntRect(0,0,0,0));
}

void ScissorLiftItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);
	
	writer << m_isActive << std::endl;
	writer << m_speed.x << std::endl;
	writer << m_speed.y << std::endl;
	writer << m_distanceMoved.x << std::endl;
	writer << m_distanceMoved.y << std::endl;
	writer << m_distanceFromPlatform << std::endl;
	writer << m_xRectangle.left << std::endl;
	writer << m_xRectangle.top << std::endl;
	writer << m_xRectangle.width << std::endl;
	writer << m_xRectangle.height << std::endl;

	m_platform.serialize(writer);
}

void ScissorLiftItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);

	reader >> m_isActive;
	reader >> m_speed.x >> m_speed.y;
	reader >> m_distanceMoved.x >> m_distanceMoved.y;
	reader >> m_distanceFromPlatform;
	reader >> m_xRectangle.left >> m_xRectangle.top >> m_xRectangle.width >> m_xRectangle.height;

	int platformID = 0;
	reader >> platformID; // Items don't parse ID's since this is usually taken care in the level loading
	m_platform.deserialize(reader);

}

bool ScissorLiftItem::onSyncedWith(Item &otherItem)
{
	return m_platform.onSyncedWith(otherItem);
}

void ScissorLiftItem::update(float deltaTime, Game &game)
{
	sf::Vector2f offset = m_platform.getPosition() - getPosition();
	m_collisionBounds = sf::FloatRect(
		offset.x,
		offset.y,
		m_platform.getCollisionBounds().width,
		m_platform.getCollisionBounds().height);
	m_distanceFromPlatform = 200;
	m_platform.update(deltaTime, game);
	m_xTexture.setPosition(getPosition().x, m_platform.getPosition().y + m_platform.getSprite().getGlobalBounds().height);
	m_xTexture.setTextureRect(sf::IntRect(0, 0, m_xSize, m_xSize));
	m_topTexture.setPosition(m_platform.getPosition().x, m_platform.getPosition().y - m_platform.getSprite().getGlobalBounds().height);
	m_topTexture.setTextureRect(sf::IntRect(0, 0, m_platform.getSprite().getGlobalBounds().width, m_platform.getSprite().getGlobalBounds().height));
	m_xTexture.setScale(1, abs(m_platform.getPosition().y + m_platform.getSprite().getGlobalBounds().height - getPosition().y) / m_xSize);
}

void ScissorLiftItem::onPositionChanged()
{
	m_platform.setPosition(sf::Vector2f(getPosition().x + (getSprite().getGlobalBounds().width / 2) - (m_platform.getSprite().getGlobalBounds().width / 2),
		getPosition().y - m_platform.getSprite().getGlobalBounds().height - m_maxDistance - 40));
}

void ScissorLiftItem::onExamine()
{
	m_examineString = "This scissor lift is named Lisa.";
}

void ScissorLiftItem::draw()
{
	Renderer::instance().drawBehind(getSprite());
	Renderer::instance().drawBehind(m_xTexture);
	Renderer::instance().drawAbove(m_topTexture);
	m_platform.draw();
}

Item* ScissorLiftItem::clone()
{
	return new ScissorLiftItem(*this);
}

sf::Vector2f ScissorLiftItem::getSpeed() const
{
	return m_platform.getSpeed();
}

sf::FloatRect ScissorLiftItem::getCollisionBounds() const
{
	return m_platform.getCollisionBounds();
}