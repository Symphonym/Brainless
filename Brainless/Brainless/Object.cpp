#include "Object.h"

Object::Object(float startX, float startY) : m_positionX(startX), m_positionY(startY), m_sprite()
{
	m_speedX = 0;
	m_speedY = 0;
	m_width = 30;
	m_height = 30;
	m_sprite.setPosition(sf::Vector2f(startX, startY));
}

Object::Object(float startX, float startY, int width, int height) : m_positionX(startX), m_positionY(startY), m_sprite(), m_width(width), m_height(height)
{
	m_speedX = 0;
	m_speedY = 0;
	m_sprite.setPosition(sf::Vector2f(startX, startY));
}

void Object::update()
{

}

sf::Sprite Object::getSprite()
{
	return m_sprite;
}

void Object::setTexture(sf::Texture texture)
{
	m_sprite.setTexture(texture);
}