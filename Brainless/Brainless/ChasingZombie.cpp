#include "ChasingZombie.h"

#define MAX_SPEED_X (float) 30
#define MAX_SPEED_Y (float) 200
#define COLLISION_WIDTH (int) 80
#define COLLISION_HEIGHT (int) 190
#define SPRITE_OFFSET_X (int) -85
#define SPRITE_OFFSET_Y (int) -50

//m_size(sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT)),
//m_maxSpeed(sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y)),

//v0.02

ChasingZombie::ChasingZombie(sf::Vector2f startPosition, int maxLengthX, Unit* target)
:
Zombie(startPosition, sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT), sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y), sf::Vector2f(SPRITE_OFFSET_X, SPRITE_OFFSET_Y), ID_ChasingZombie),
m_maxWalkLenght(maxLengthX),
m_direction(dir_noDirection),
m_currentLength(0),
m_animState(anim_noAnimation),
m_target(target),
m_chaseState(chase_idle),
m_homePosition(startPosition)
{

}

void ChasingZombie::updateTask(float deltaTime)
{
	float speedTurnAround = 10;
	float speedStartAcc = 300;
	float speedNormalAcc = 100;
	float speedSlowDown = 6;

	float startAccBreakpoint = 150;
	float minSpeedBeforeStop = 10;

	float chaseDistance = 500;
	bool slowDown = true;
	
	//Chasing Zombie's AI

	//not done
	//not far away from home
	//if (abs(m_homePosition.x - m_position.x) < m_maxWalkLenght)
	//{

		sf::Vector2f length = m_target->getPosition() - getPosition();
		if (length.x < 0 && -chaseDistance < length.x)
		{
			m_chaseState = chase_chase;
			m_direction = dir_left;
		}
		else if (0 < length.x && length.x < chaseDistance)
		{
			m_chaseState = chase_chase;
			m_direction = dir_right;
		}
		else if (m_chaseState == chase_chase)
		{
			m_chaseState = chase_returnHome;
		}

	//}
	//else
	//{
	//	m_chaseState == chase_farFromHome;
	//	m_direction = dir_noDirection;
	//}

	if (m_chaseState == chase_returnHome)
	{
		if (/*home*/ true)
		{
			m_chaseState = chase_idle;
			m_direction = dir_noDirection;
		}
		//else return home ai
		
	}

	//movement
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

void ChasingZombie::updateAnimation(float deltaTime)
{

	if (m_direction == dir_noDirection)
	{
		if (m_animState != anim_idle)
		{
			m_sprite = &m_spriteSheets[0];
			m_animation.stillFrame(0, 2);
			m_animState = anim_idle;
		}
	}
	if (m_animState != anim_walking)
	{
		m_sprite = &m_spriteSheets[0];
		m_animation.loop(0, 7, 2, 3);
		m_animState = anim_walking;
	}


	updateSpriteDirection();

	m_sprite->setTextureRect(m_animation.getRectangle(deltaTime));
}

int ChasingZombie::getWalkLenght()
{
	return m_maxWalkLenght;
}