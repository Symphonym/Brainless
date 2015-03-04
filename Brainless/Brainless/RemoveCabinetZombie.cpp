#include "RemoveCabinetZombie.h"
#include "ItemDatabase.h"
#include <iostream>

#define MAX_SPEED_X (float) 200
#define MAX_SPEED_Y (float) 200
#define COLLISION_WIDTH (int) 80
#define COLLISION_HEIGHT (int) 190
#define SPRITE_OFFSET_X (int) -85
#define SPRITE_OFFSET_Y (int) -50


//m_size(sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT)),
//m_maxSpeed(sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y)),

//v0.02

RemoveCabinetZombie::RemoveCabinetZombie(sf::Vector2f startPosition, int Texture, sf::Vector2f target)
:
Zombie(startPosition, sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT), sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y), sf::Vector2f(SPRITE_OFFSET_X, SPRITE_OFFSET_Y), ID_CabinetZombie, Texture),
m_direction(dir_right),
m_target(target)
{
	m_isDamaging = false;
}

void RemoveCabinetZombie::serialize(std::ofstream &writer) const
{
	Unit::serialize(writer);

	writer << m_textureId << std::endl;

	writer << m_target.x << std::endl;
	writer << m_target.y << std::endl;

	writer << static_cast<int>(m_spriteDirection) << std::endl;
	writer << static_cast<int>(m_direction) << std::endl;
}
void RemoveCabinetZombie::deserialize(std::ifstream &reader)
{
	Unit::deserialize(reader);
	//HEJ
	reader >> m_textureId;

	reader >> m_target.x >> m_target.y;

	int spriteDirType = 0;
	reader >> spriteDirType;
	m_spriteDirection = static_cast<Direction>(spriteDirType);


	int dirType = 0;
	reader >> dirType;
	m_direction = static_cast<Direction>(dirType);
}

void RemoveCabinetZombie::updateTask(float deltaTime)
{

		float speedTurnAround = 10;
		float speedStartAcc = 300;
		float speedNormalAcc = 100;
		float speedSlowDown = 6;

		float startAccBreakpoint = 150;
		float minSpeedBeforeStop = 10;

		bool slowDown = true;
		if (m_direction != dir_noDirection)
		{
			animation_walking();
			if (m_position.x < m_target.x)
			{
				m_direction = dir_right;
			}
			else
			{
				m_direction = dir_left;
			}
		}
		else
		{	
			animation_idle();
		}
		////Note for simplicity: This is practically physics code and could be used for all unit classes instead of copied.
		////Left
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

void RemoveCabinetZombie::onCollideWithItem(Item &item)
{
	if (item.getName() == "Cabinet")
	{
		m_direction = dir_noDirection;
	}
}

void RemoveCabinetZombie::electricPuddle()
{
	std::cout << "dangerousPuddle" << std::endl;
	m_direction = dir_noDirection;
	//animation_idle();
	//play dialog
}
void RemoveCabinetZombie::safePuddle()
{
	std::cout << "safePuddle" << std::endl;
	if (m_position.x < m_target.x)
	{
		m_direction = dir_right;
	}
	else
	{
		m_direction = dir_left;
	}
}