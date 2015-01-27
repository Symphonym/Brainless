/*



*/
#include "Player.h"
#include <iostream>
using namespace std;
Player::Player(float startX, float startY, float maxSpeedX, float maxSpeedY)
:
Unit(startX, startY, maxSpeedX, maxSpeedY),
m_state(idle),
m_spriteDirection(right),
m_inputDirection(right),
m_animation(60,90) //storleken på varje bild i texturesheet
{
	m_sprite.setPosition(sf::Vector2f(startX, startY));
	m_animation.loop(0, 3, 0, 5);
}

Player::Player(float startX, float startY, int width, int height, float maxSpeedX, float maxSpeedY) 
: 
Unit(startX, startY, width, height, maxSpeedX, maxSpeedY), 
m_state(idle),
m_spriteDirection(right),
m_inputDirection(right),
m_animation(60,90)
{
	m_sprite.setPosition(sf::Vector2f(startX, startY));
	m_animation.loop(0, 3, 0, 5);
}

void Player::checkPlayerInput()
{
	bool slowDown = true;
	//Left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		if (20 < m_speedX) //wrong direcion - slow character
		{
			m_accelerationX = -m_speedX * 8;
		}
		else m_accelerationX = -200;

		slowDown = false;
		m_inputDirection = left;
	}
	//Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (m_speedX < -20) //wrong direcion - slow character
		{
			m_accelerationX = -m_speedX * 8;
		}
		else m_accelerationX = 200;
		slowDown = false;
		m_inputDirection = right;
	}
	if (slowDown)
	{
		//small values = stop totally
		if (m_speedX < 10 && m_speedX > -10) 
		{
			m_speedX = 0;
			m_accelerationX = 0;
		}
		//slow
		else m_accelerationX = -m_speedX * 6;
	}
	//Jump
	if (!m_inAir && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		m_speedY = -600;
		m_inAir = true;
	}
}

// nästa steg fixa kolla enbart state, sen kolla scale
void Player::updateAnimation(float deltaTime)
{
	float runBreakpoint = m_maxSpeedX * 2 / 3;
	if (m_inAir)
	{
		//JUMP
		if (m_speedY < 0)
		{
			if (m_state != jump)
			{
				m_animation.stillFrame(1, 3);
				m_state = jump;
			}
		}
		//FALL
		else if (0 <= m_speedY)
		{
			if (m_state != fall)
			{
				m_animation.stillFrame(7, 3);
				m_state = fall;
			}
		}
	}
	//IDLE
	else if (m_speedX == 0)
	{
		if (m_state != idle)
		{
			m_animation.loop(0, 3, 0, 5);
			m_state = idle;
		}
	}
	//SLIDE
	else if (m_speedX < 0 && m_inputDirection == right || 0 < m_speedX && m_inputDirection == left)
	{
		if (m_state != slide)
		{
			m_animation.stillFrame(4,0);
			m_state = slide;
		}
	}
	//RUN
	else if (runBreakpoint <= m_speedX || m_speedX <= -runBreakpoint)
	{
		if (m_state != run)
		{
			m_animation.loop(4, 7, 2, 10);
			m_state = run;
		}
		m_animation.setSpeed(calcFrameSpeed(10, 20, runBreakpoint, m_maxSpeedX, m_speedX));
		
	}
	//WALK
	else if (m_speedX != 0)
	{
		if (m_state != walk)
		{
			m_animation.loop(0, 3, 1, 5);
			m_state = walk;
		}
		m_animation.setSpeed(calcFrameSpeed(5, 20, 0, runBreakpoint, m_speedX));
	}


/*	slide, //(turning while running)
		jump,
		fall*/

	//blev rörigt här, beroende på vart bildplacering gentemot collisionsplacering så ändras setpoision ev. någon annan stans.
	//men just nu ändras set position i Unit.cpp genom samma position som collisionsplaceringen så setposition gentemot scale(-1,1) behöver ändras varje gång.

	if (m_spriteDirection == left)
	{
		if (0 < m_speedX)
		{
			m_spriteDirection = right;
			m_sprite.setScale(1, 1);
		}
		m_sprite.setScale(-1, 1);
		m_sprite.setPosition(m_positionX + m_animation.getWidth(), m_positionY);
	}
	else if (m_spriteDirection == right)
	{
		if (m_speedX < 0)
		{
			m_spriteDirection = left;
			m_sprite.setScale(-1, 1);
			m_sprite.setPosition(m_positionX + m_animation.getWidth(), m_positionY);
		}
		m_sprite.setScale(1, 1);
	}



	m_sprite.setTextureRect(m_animation.getRectangle(deltaTime));
}

/*
	returns speed closer minSpeed when value is closer to useMinValue
	returns speed closer maxSpeed when value is closer to useMaxValue
	useMinValue can be higher than useMaxValue
*/
float Player::calcFrameSpeed(float minSpeed, float maxSpeed, float useMinValue, float useMaxValue, float value)
{
	if (minSpeed == maxSpeed) return minSpeed;
	
	//"Reverted"
	if (useMaxValue < useMinValue)
	{
		if (value < useMaxValue) return maxSpeed;
		else if (useMinValue < value) return minSpeed;
		float b = useMinValue - useMaxValue;
		float a = abs(value) - useMaxValue;
		if (a == 0) return maxSpeed;
		return maxSpeed - (maxSpeed - minSpeed)*(b / a);
	}
	//Normal
	if (useMaxValue < value) return maxSpeed;
	else if (value < useMinValue) return minSpeed;
	float a = useMaxValue - useMinValue;
	float b = abs(value) - useMinValue;
	if (a == 0) return minSpeed;
	return minSpeed + (maxSpeed-minSpeed)*(b / a);
}

