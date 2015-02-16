#include "ScissorLift.h"
#include "Renderer.h"
#include "ResourceLoader.h"

ScissorLiftItem::ScissorLiftItem(sf::Vector2f speed, float maxDistance, int id)
:
Item("ScissorLift", "liftBottom", id),
m_speed(speed),
m_maxDistanceMoved(maxDistance),
m_isActive(true),
m_maxDistance(maxDistance),
m_platform(speed,maxDistance,id)
{
	m_collidable = true;
	m_solid = false;
	m_lootable = false;

	m_collisionOffset = sf::Vector2f(0, 0);
	m_xTexture = sf::Sprite(ResourceLoader::instance().retrieveTexture("liftX"),
		sf::IntRect(0, 0, m_xSize, m_xSize));
	m_topTexture = sf::Sprite(ResourceLoader::instance().retrieveTexture("liftTop"),
		sf::IntRect(0, 0, m_platform.getSprite().getGlobalBounds().width, m_platform.getSprite().getGlobalBounds().height));
}

void ScissorLiftItem::serialize(std::ofstream &writer) const
{
	m_platform.serialize(writer);
}

void ScissorLiftItem::deserialize(std::ifstream &reader)
{
	m_platform.deserialize(reader);
}

bool ScissorLiftItem::onSyncedWith(Item &otherItem)
{
	return m_platform.onSyncedWith(otherItem);
}

void ScissorLiftItem::update(float deltaTime, Game &game)
{
	m_collisionSize.x = m_platform.getCollisionBounds().width;
	m_collisionSize.y = m_platform.getCollisionBounds().height;
	m_collisionOffset = m_platform.getPosition() - getPosition();
	m_distanceFromPlatform = 200;
	m_platform.update(deltaTime, game);
	m_xTexture.setPosition(getPosition().x, m_platform.getPosition().y + m_platform.getSprite().getGlobalBounds().height);
	m_topTexture.setPosition(m_platform.getPosition().x, m_platform.getPosition().y - m_platform.getSprite().getGlobalBounds().height);
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
