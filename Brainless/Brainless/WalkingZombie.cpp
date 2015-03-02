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

WalkingZombie::WalkingZombie(sf::Vector2f startPosition, int maxDisparityX, int Texture)
:
Zombie(startPosition, sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT), sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y), sf::Vector2f(SPRITE_OFFSET_X, SPRITE_OFFSET_Y), ID_WalkingZombie,Texture),
m_maxPositionX(maxDisparityX + startPosition.x),
m_walkLenght(maxDisparityX),
m_direction(dir_right),
m_minPositionX(startPosition.x),
m_currentDisparity(0)
{
	if (maxDisparityX < 0)
	{
		m_maxPositionX = startPosition.x;
		m_minPositionX = m_maxPositionX+maxDisparityX;
		m_currentDisparity = -1 * maxDisparityX;
	}
	animation_walking();
}

void WalkingZombie::serialize(std::ofstream &writer) const
{
	Unit::serialize(writer);

	writer << static_cast<int>(m_animState) << std::endl;
	writer << static_cast<int>(m_spriteDirection) << std::endl;
	writer << m_maxPositionX << std::endl;
	writer << m_minPositionX << std::endl;
	writer << m_walkLenght << std::endl;
	writer << m_currentDisparity << std::endl;
	writer << static_cast<int>(m_direction) << std::endl;
}
void WalkingZombie::deserialize(std::ifstream &reader)
{
	Unit::deserialize(reader);

	int animType = 0;
	reader >> animType;
	m_animState = static_cast<AnimationState>(animType);

	int spriteDirType = 0;
	reader >> spriteDirType;
	m_spriteDirection = static_cast<Direction>(spriteDirType);

	reader >> m_maxPositionX >> m_minPositionX;
	reader >> m_walkLenght;
	reader >> m_currentDisparity;

	int dirType = 0;
	reader >> dirType;
	m_direction = static_cast<Direction>(dirType);
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

void WalkingZombie::wallLeft()
{
	m_direction = dir_right;
}

void WalkingZombie::wallRight()
{
	m_direction = dir_left;
}


int WalkingZombie::getWalkLenght()
{
	return m_walkLenght;
}