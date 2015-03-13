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

ChasingZombie::ChasingZombie(sf::Vector2f startPosition, int maxLengthX, int Texture, int syncID)
:
Zombie(startPosition, sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT), sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y), sf::Vector2f(SPRITE_OFFSET_X, SPRITE_OFFSET_Y), ID_ChasingZombie,Texture, syncID),
m_maxWalkLenght(abs(maxLengthX)),
m_direction(dir_noDirection),
m_currentLength(0),
m_target(s_playerPointer),
m_chaseState(chase_idle),
m_homePosition(startPosition)
{
	animation_idle();
}


void ChasingZombie::serialize(std::ofstream &writer) const
{
	Unit::serialize(writer);

	writer << m_textureId << std::endl;

	writer << m_homePosition.x << std::endl;
	writer << m_homePosition.y << std::endl;
	//writer << static_cast<int>(m_animState) << std::endl;
	writer << m_maxWalkLenght << std::endl;
	writer << m_currentLength << std::endl;
	writer << static_cast<int>(m_direction) << std::endl;
	writer << static_cast<int>(m_chaseState) << std::endl;
}
void ChasingZombie::deserialize(std::ifstream &reader)
{
	Unit::deserialize(reader);

	reader >> m_textureId;

	reader >> m_homePosition.x >> m_homePosition.y;

	//int animType = 0;
	//reader >> animType;
	//m_animState = static_cast<AnimationState>(animType); //kan bli texture 0 vid idle -> idle, ingen animation byts fixa lite senare

	reader >> m_maxWalkLenght;
	reader >> m_currentLength;

	int dirType = 0;
	reader >> dirType;
	m_direction = static_cast<Direction>(dirType);

	int chaseType = 0;
	reader >> chaseType;
	m_chaseState = static_cast<ChasingState>(chaseType);
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
	float lookDistance = 450;
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

	if (m_isDamaging)
	{
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
			{
				if (m_target->getPosition().x < m_position.x)
					m_direction = dir_left;
				else
					m_direction = dir_right;
				animation_walkingSlow();
			}
			//stare
			else
			{
				m_specialSpriteDirection = true;
				if (m_target->getPosition().x < m_position.x)
				{
					m_spriteDirection = dir_left;
				}
				else
				{
					m_spriteDirection = dir_right;
				}
				animation_idleSlow();
			}
			//return home
			else
			{
				if (m_position.x + 5 < m_homePosition.x)
				{
					m_direction = dir_right;
					animation_walkingSlow();
				}
				else if (m_position.x - 5 > m_homePosition.x)
				{
					m_direction = dir_left;
					animation_walkingSlow();
				}
				//isHome
				else
				{
					m_position.x = m_homePosition.x;
					m_direction = dir_noDirection;
					animation_idleSlow();
				}
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

int ChasingZombie::getWalkLength()
{
	return m_maxWalkLenght;
}