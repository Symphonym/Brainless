/*



*/
#include "Player.h"
#include <iostream>
using namespace std;
Player::Player(float startX, float startY, float maxSpeedX, float maxSpeedY)
:
Unit(startX, startY, maxSpeedX, maxSpeedY),
m_state(idle),
m_direction(right),
m_animation(60,90) //storleken på varje bild i texturesheet
{
	m_sprite.setPosition(sf::Vector2f(startX, startY));
	m_animation.loop(0, 3, 0, 5);
}

Player::Player(float startX, float startY, int width, int height, float maxSpeedX, float maxSpeedY) 
: 
Unit(startX, startY, width, height, maxSpeedX, maxSpeedY), 
m_state(idle),
m_direction(right),
m_animation(60,90)
{
	m_sprite.setPosition(sf::Vector2f(startX, startY));
	m_animation.loop(0, 3, 0, 5);
}

void Player::checkPlayerInput()
{
	//dåligt sätt att hantera keyboard, funkar bara om knappen hålls inne vid just dessa if's
	//inte snyggt vid vändningar
	bool slide = true;
	//Left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		if (20 < m_speedX)
		{
			m_accelerationX = -m_speedX * 10;
		}
		else m_accelerationX = -200;
		if (-20 <= m_speedX && m_speedX <= 20)
		{
	//		m_speedX -= 100;
		}
		slide = false;

	}
	//Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (m_speedX < -20)
		{
			m_accelerationX = -m_speedX * 10;
		}
		else m_accelerationX = 200;
		if (-20 <= m_speedX && m_speedX <= 20)
		{
		//	m_speedX += 100;
		}
		slide = false;
	}
	if (slide)
	{
		//så små värden, sluta slida
		if (m_speedX < 10 && m_speedX > -10)
		{
			m_speedX = 0;
			m_accelerationX = 0;
		}
		//slide
		else m_accelerationX = -m_speedX * 10;
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
	if (m_speedX == 0)
	{
		if (m_state != idle)
		{
			m_animation.loop(0, 3, 0, 5);
			m_state = idle;
		}
	}
	else if (runBreakpoint <= m_speedX || m_speedX <= -runBreakpoint)
	{
		if (m_state != run)
		{
			m_animation.loop(4, 7, 2, 10);
			m_state = run;
		}
		m_animation.setSpeed(calcSpeed(10, 20, runBreakpoint, m_maxSpeedX, m_speedX));
		
	}
	else if (m_speedX != 0)
	{
		if (m_state != walk)
		{
			m_animation.loop(0, 3, 1, 5);
			m_state = walk;
		}
		m_animation.setSpeed(calcSpeed(5, 20, 0, runBreakpoint, m_speedX));
	}

	//blev rörigt här, beroende på vart bildplacering gentemot collisionsplacering så ändras setpoision ev. någon annan stans.
	//men just nu ändras set position i Unit.cpp genom samma position som collisionsplaceringen så setposition gentemot scale(-1,1) behöver ändras varje gång.

	if (m_direction == left)
	{
		if (0 < m_speedX)
		{
			m_direction = right;
			m_sprite.setScale(1, 1);
		}
		m_sprite.setScale(-1, 1);
		m_sprite.setPosition(m_positionX + m_animation.getWidth(), m_positionY);
	}
	else if (m_direction == right)
	{
		if (m_speedX < 0)
		{
			m_direction = left;
			m_sprite.setScale(-1, 1);
			m_sprite.setPosition(m_positionX + m_animation.getWidth(), m_positionY);
		}
		m_sprite.setScale(1, 1);
	}



	m_sprite.setTextureRect(m_animation.getRectangle(deltaTime));
}

float Player::calcSpeed(float minSpeed, float maxSpeed, float minValue, float maxValue, float value)
{
	//if value < minValue || maxValue < value, throw error
	float a = maxValue-minValue;
	float b = abs(value)-minValue;
	if (b == 0) return minSpeed;
	
	return minSpeed + maxSpeed*(b / a);
}

