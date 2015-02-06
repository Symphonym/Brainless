#include "IdleZombie.h"

#define MAX_SPEED_X (float) 200
#define MAX_SPEED_Y (float) 200
#define COLLISION_WIDTH (int) 80
#define COLLISION_HEIGHT (int) 190
#define SPRITE_OFFSET_X (int) -85
#define SPRITE_OFFSET_Y (int) -50

//m_size(sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT)),
//m_maxSpeed(sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y)),

//v0.02

IdleZombie::IdleZombie(sf::Vector2f startPosition, Direction startDirection)
:
Zombie(startPosition, sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT), sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y), sf::Vector2f(SPRITE_OFFSET_X, SPRITE_OFFSET_Y), ID_IdleZombie),
m_animState(anim_noAnimation)
{
	m_spriteDirection = startDirection;
}

void IdleZombie::updateTask(float deltaTime)
{

}

void IdleZombie::updateAnimation(float deltaTime)
{

	if (m_animState != anim_idle)
	{
		m_sprite = &m_spriteSheets[0];
		m_animation.loop(0, 7, 1, 5);
		m_animState = anim_idle;
	}

	updateSpriteDirection();

	m_sprite->setTextureRect(m_animation.getRectangle(deltaTime));
}
