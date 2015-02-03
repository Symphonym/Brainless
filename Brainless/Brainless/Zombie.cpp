#include "Zombie.h"

#define MAX_SPEED_X (float) 200
#define MAX_SPEED_Y (float) 200
#define COLLISION_WIDTH (int) 80
#define COLLISION_HEIGHT (int) 190
#define SPRITE_OFFSET_X (int) -85
#define SPRITE_OFFSET_Y (int) -50

//m_size(sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT)),
//m_maxSpeed(sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y)),

//v0.02

Zombie::Zombie(sf::Vector2f startPosition) 
: 
Unit(startPosition, sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT), sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y), sf::Vector2f(SPRITE_OFFSET_X, SPRITE_OFFSET_Y)),
m_animState(noAnimation),
m_spriteDirection(right)
{

}

void Zombie::updateAnimation(float deltaTime)
{
	
	if (m_animState != idle)
	{
		m_sprite = &m_spritSheets[0];
		m_animation.stillFrame(0, 0);
		m_animState = idle;
	}
	
	//Sprite mirroring and offset.
	if (m_spriteDirection == left)
	{
		if (0 < m_speed.x)
		{
			m_spriteDirection = right;
			m_sprite->setScale(1, 1);
			m_sprite->setPosition(sf::Vector2f(m_position.x + m_spriteOffset.x, m_position.y + m_spriteOffset.y + m_spriteOffset.y));

		}
		m_sprite->setScale(-1, 1);
		m_sprite->setPosition(m_position.x + m_spriteOffset.x + m_animation.getWidth(), m_position.y + m_spriteOffset.y);
	}
	else if (m_spriteDirection == right)
	{
		if (m_speed.x < 0)
		{
			m_spriteDirection = left;
			m_sprite->setScale(-1, 1);
			m_sprite->setPosition(m_position.x + m_spriteOffset.x + m_animation.getWidth(), m_position.y + m_spriteOffset.y);
		}
		m_sprite->setScale(1, 1);
		m_sprite->setPosition(sf::Vector2f(m_position.x + m_spriteOffset.x, m_position.y + m_spriteOffset.y));
	}



	m_sprite->setTextureRect(m_animation.getRectangle(deltaTime));
}
