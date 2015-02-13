#include "ChasingZombie.h"
#include <iostream>

#define MAX_SPEED_X (float) 50
#define MAX_SPEED_Y (float) 200
#define COLLISION_WIDTH (int) 80
#define COLLISION_HEIGHT (int) 190
#define SPRITE_OFFSET_X (int) -85
#define SPRITE_OFFSET_Y (int) -50

//m_size(sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT)),
//m_maxSpeed(sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y)),

//v0.02

ChasingZombie::ChasingZombie(sf::Vector2f startPosition, int maxLengthX)
:
Zombie(startPosition, sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT), sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y), sf::Vector2f(SPRITE_OFFSET_X, SPRITE_OFFSET_Y), ID_ChasingZombie),
m_maxWalkLenght(abs(maxLengthX)),
m_direction(dir_noDirection),
m_currentLength(0),
m_animState(anim_noAnimation),
m_target(s_playerPointer),
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

	float chaseDistance = 300;
	float lookDistance = 500;
	bool slowDown = true;
	
	//Chasing Zombie's AI
	/*
		todo
		jaga spelare om står vid endpunkt och spelare kommer från andra hållet
		om är påväg hem och ser spelare, fortsätt hem istället för att stanna och stirra

	*/
	/*
		special spriteDirection = false;
		if(target in sight)
		{
			if(target looks tasty && NOT far from Home)
			{
				chasemode
				chase direction
			}
			else
			{
				special spriteDirection = true;
				look at player
				stand still
			}
		}
		else
		{
			if(home)
			{
				stand still;
			}
			else
			{
				calculate direction
			}
		}
		do movement
	*/
	m_target = s_playerPointer;
	if (m_target != nullptr)
	{
		sf::Vector2f length = m_target->getPosition() - m_position;
		float distance = sqrt(pow(length.x, 2) + pow(length.y, 2));

		m_specialSpriteDirection = false;
		m_direction = dir_noDirection;
		//chasing zombie Ai

		//target in sight
		if (distance < lookDistance)
			//target looks tasty && not far from home
		if (distance < chaseDistance && abs(m_position.x - m_homePosition.x) < m_maxWalkLenght)
		if (m_target->getPosition().x < m_position.x)
			m_direction = dir_left;
		else
			m_direction = dir_right;
		//stare
		else
		{
			m_specialSpriteDirection = true;
			if (m_target->getPosition().x < m_position.x)
				m_spriteDirection = dir_left;
			else
				m_spriteDirection = dir_right;
		}
		//return home
		else
		if (m_position.x + 5 < m_homePosition.x)
			m_direction = dir_right;
		else if (m_position.x - 5 > m_homePosition.x)
			m_direction = dir_left;
		//isHome
		else
		{
			m_position.x = m_homePosition.x;
			m_direction = dir_noDirection;
		}
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
			m_animation.loop(0, 7, 3, 3);
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