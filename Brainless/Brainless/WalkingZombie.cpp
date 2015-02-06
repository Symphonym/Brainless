#include "WalkingZombie.h"
#define MAX_SPEED_X (float) 200
#define MAX_SPEED_Y (float) 200
#define COLLISION_WIDTH (int) 80
#define COLLISION_HEIGHT (int) 190
#define SPRITE_OFFSET_X (int) -85
#define SPRITE_OFFSET_Y (int) -50

//m_size(sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT)),
//m_maxSpeed(sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y)),

//v0.02

WalkingZombie::WalkingZombie(sf::Vector2f startPosition, int maxDisparityX)
:
Zombie(startPosition, sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT), sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y), sf::Vector2f(SPRITE_OFFSET_X, SPRITE_OFFSET_Y), ID_WalkingZombie),
m_maxPositionX(maxDisparityX + startPosition.x),
m_walkLenght(maxDisparityX),
m_direction(dir_right),
m_minPositionX(startPosition.x),
m_currentDisparity(0),
m_animState(anim_noAnimation)
{
	if (maxDisparityX < 0)
	{
		m_maxPositionX = startPosition.x;
		m_minPositionX = m_maxPositionX+maxDisparityX;
		m_currentDisparity = -1 * maxDisparityX;
	}
}

void WalkingZombie::updateTask(float deltaTime)
{
	float speedTurnAround = 10;
	float speedStartAcc = 300;
	float speedNormalAcc = 100;
	float speedSlowDown = 6;

	float startAccBreakpoint = 150;
	float minSpeedBeforeStop = 10;

	bool slowDown = true;
	//Walking Zombie's AI
	if (m_position.x < m_minPositionX)
		m_direction = dir_right;
	else if (m_maxPositionX < m_minPositionX + (m_position.x - m_minPositionX))
		m_direction = dir_left;
	//Note for simplicity: This is practically physics code and could be used for all unit classes instead of copied.
	//Left
	if (m_direction == dir_left)
	{
		if (20 < m_speed.x) //wrong direcion - slow character
		{
			m_acceleration.x = -m_speed.x * speedTurnAround;
		}
		else
		{
			if (abs(m_speed.x) < startAccBreakpoint)
				m_acceleration.x = -speedStartAcc;
			else
				m_acceleration.x = -speedNormalAcc;
		}
		slowDown = false;
	}
	//Right
	if (m_direction == dir_right)
	{
		if (m_speed.x < -20) //wrong direcion - slow character
		{
			m_acceleration.x = -m_speed.x * speedTurnAround;
		}
		else
		{
			if (abs(m_speed.x) < startAccBreakpoint)
				m_acceleration.x = speedStartAcc;
			else
				m_acceleration.x = speedNormalAcc;
		}
		slowDown = false;
	}
	if (slowDown)
	{
		//small values = stop totally
		if (m_speed.x < minSpeedBeforeStop && m_speed.x > -minSpeedBeforeStop)
		{
			m_speed.x = 0;
			m_acceleration.x = 0;
		}
		//slow
		else m_acceleration.x = -m_speed.x * speedSlowDown;
	}
}

void WalkingZombie::updateAnimation(float deltaTime)
{

	if (m_animState != anim_walking)
	{
		m_sprite = &m_spriteSheets[0];
		m_animation.loop(0, 7, 0, 5);
		m_animState = anim_walking;
	}

	updateSpriteDirection();

	m_sprite->setTextureRect(m_animation.getRectangle(deltaTime));
}

int WalkingZombie::getWalkLenght()
{
	return m_walkLenght;
}