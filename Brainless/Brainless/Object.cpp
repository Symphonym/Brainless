#include "Object.h"
#include "Renderer.h"
#include "ResourceLoader.h"

Object::Object(float startX, float startY) : m_positionX(startX), m_positionY(startY), m_sprite()
{
	m_width = 32;
	m_height = 32;
	m_sprite.setPosition(sf::Vector2f(startX, startY));
}

Object::Object(float startX, float startY, int width, int height) : m_positionX(startX), m_positionY(startY), m_sprite(), m_width(width), m_height(height)
{
	m_sprite.setPosition(sf::Vector2f(startX, startY));
}

void Object::update()
{

}

sf::Sprite Object::getSprite()
{
	return m_sprite;
}

void Object::setTexture(sf::Texture& texture)
{
	m_sprite.setTexture(texture);
}

void Object::draw()
{
	Renderer::instance().draw(m_sprite);

}