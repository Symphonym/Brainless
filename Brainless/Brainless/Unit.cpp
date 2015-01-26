#include "Unit.h"
#include "Renderer.h"

Unit::Unit(float startX, float startY) : m_positionX(startX), m_positionY(startY), m_speedX(0), m_speedY(0), m_accelerationX(0), m_accelerationY(0), m_width(200), m_height(200)
{
	m_sprite.setPosition(sf::Vector2f(startX, startY));
}
Unit::Unit(float startX, float startY, int width, int height) : m_positionX(startX), m_positionY(startY), m_speedX(0), m_speedY(0), m_accelerationX(0), m_accelerationY(0)
, m_width(width), m_height(height)
{
	m_sprite.setPosition(sf::Vector2f(startX, startY));
}
void updateMovement(float gravity)
{
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D));


}

sf::Sprite Unit::getSprite()
{
	return m_sprite;
}

void Unit::setTexture(sf::Texture& texture)
{
	m_sprite.setTexture(texture);
}

void Unit::draw()
{
	Renderer::instance().draw(m_sprite);
}

