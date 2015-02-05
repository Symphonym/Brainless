#include "Unit.h"
#include "Renderer.h"

#define SPRITESIZE 256 //storleken på varje bild i texturesheet

//V0.02
Unit::Unit(sf::Vector2f startPosition, sf::Vector2f size, sf::Vector2f maxSpeed, sf::Vector2f spriteOffset)
:
m_position(startPosition),
m_speed(sf::Vector2f(0, 0)),
m_acceleration(sf::Vector2f(0, 0)),
m_size(size),
m_maxSpeed(maxSpeed),
m_inAir(false),
m_animation(SPRITESIZE, SPRITESIZE),
m_spriteOffset(spriteOffset),
m_spriteDirection(dir_right),
m_specialSpriteDirection(false)
{

}
void Unit::updateMovement(float gravity, float deltaTime)
{
	float accelYtrue;
	if (m_inAir)
	{

		accelYtrue = m_acceleration.y + gravity;
	}
	else
	{
		m_speed.y = 0;
		m_acceleration.y = 0;
		accelYtrue = 0;
	}

	m_speed.x += m_acceleration.x * deltaTime;
	m_speed.y += accelYtrue * deltaTime;
	if (m_speed.x > m_maxSpeed.x) m_speed.x = m_maxSpeed.x;
	if (m_speed.y > m_maxSpeed.y) m_speed.y = m_maxSpeed.y;
	if (m_speed.x < -m_maxSpeed.x) m_speed.x = -m_maxSpeed.x;
	if (m_speed.y < -m_maxSpeed.y) m_speed.y = -m_maxSpeed.y;
	m_position.x += m_speed.x * deltaTime;
	m_position.y += m_speed.y * deltaTime;
}

void Unit::setStatus(bool inAir)
{
	m_inAir = inAir;
}
void Unit::setPosition(sf::Vector2f position)
{
	m_position = position;
}
void Unit::setSpeed(sf::Vector2f speed)
{
	m_speed = speed;
}
void Unit::setAcceleration(sf::Vector2f acceleration)
{
	m_acceleration = acceleration;
}

sf::Sprite Unit::getSprite()
{
	return (*m_sprite);
}

void Unit::setTexture(int index, sf::Texture& texture)
{
	m_sprite->setTexture(texture);
	//error sheet.size() < 1
	//error index < 0
	if (index < m_spriteSheets.size())
	{
		m_spriteSheets[index].setTexture(texture);
	}
}

void Unit::addTexture(sf::Texture& texture)
{
	m_spriteSheets.push_back(sf::Sprite(texture));
	m_sprite = &m_spriteSheets[0];
}

void Unit::draw()
{
	Renderer::instance().drawDepth((*m_sprite));
}

void Unit::updateSpriteDirection()
{
	if (!m_specialSpriteDirection)
	{
		//Sprite mirroring and offset.
		if (m_spriteDirection == dir_left)
		{
			if (0 < m_speed.x)
			{
				m_spriteDirection = dir_right;
				m_sprite->setScale(1, 1);
				m_sprite->setPosition(sf::Vector2f(m_position.x + m_spriteOffset.x, m_position.y + m_spriteOffset.y + m_spriteOffset.y));

			}
			m_sprite->setScale(-1, 1);
			m_sprite->setPosition(m_position.x + m_spriteOffset.x + m_animation.getWidth(), m_position.y + m_spriteOffset.y);
		}
		else if (m_spriteDirection == dir_right)
		{
			if (m_speed.x < 0)
			{
				m_spriteDirection = dir_left;
				m_sprite->setScale(-1, 1);
				m_sprite->setPosition(m_position.x + m_spriteOffset.x + m_animation.getWidth(), m_position.y + m_spriteOffset.y);
			}
			m_sprite->setScale(1, 1);
			m_sprite->setPosition(sf::Vector2f(m_position.x + m_spriteOffset.x, m_position.y + m_spriteOffset.y));
		}
		else
		{
			m_sprite->setScale(1, 1);
			m_sprite->setPosition(sf::Vector2f(m_position.x + m_spriteOffset.x, m_position.y + m_spriteOffset.y));
		}
	}
	else
	{
		if (m_spriteDirection == dir_left)
		{
			m_sprite->setScale(-1, 1);
			m_sprite->setPosition(m_position.x + m_spriteOffset.x + m_animation.getWidth(), m_position.y + m_spriteOffset.y);
		}
		else if (m_spriteDirection == dir_right)
		{
			m_sprite->setScale(1, 1);
			m_sprite->setPosition(sf::Vector2f(m_position.x + m_spriteOffset.x, m_position.y + m_spriteOffset.y));
		}
		else
		{
			m_sprite->setScale(1, 1);
			m_sprite->setPosition(sf::Vector2f(m_position.x + m_spriteOffset.x, m_position.y + m_spriteOffset.y));
		}
	}

}

bool Unit::getInAir() const
{
	return m_inAir;
}
sf::Vector2f Unit::getPosition() const
{
	return m_position;
}
sf::Vector2f Unit::getSpeed() const
{
	return m_speed;
}
sf::Vector2f Unit::getAcceleration() const
{
	return m_acceleration;
}
sf::Vector2f Unit::getSize() const
{ 
	return m_size;
}
sf::FloatRect Unit::getCollisionRect()
{
	return sf::FloatRect(m_position, m_size);
}

