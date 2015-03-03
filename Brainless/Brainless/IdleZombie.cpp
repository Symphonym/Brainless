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

IdleZombie::IdleZombie(sf::Vector2f startPosition, Direction startDirection, int Texture)
:
Zombie(startPosition, sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT), sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y), sf::Vector2f(SPRITE_OFFSET_X, SPRITE_OFFSET_Y), ID_IdleZombie, Texture)
{
	m_spriteDirection = startDirection;
	m_specialSpriteDirection = true;
	animation_idle();
}


void IdleZombie::serialize(std::ofstream &writer) const
{
	Unit::serialize(writer);

	writer << m_textureId << std::endl;
	writer << static_cast<int>(m_animState) << std::endl;
}
void IdleZombie::deserialize(std::ifstream &reader)
{
	Unit::deserialize(reader);

	reader >> m_textureId;

	int animType = 0;
	reader >> animType;
	m_animState = static_cast<AnimationState>(animType);
}


void IdleZombie::updateTask(float deltaTime)
{

}
