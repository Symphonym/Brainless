/*



*/
#include "Player.h"
#include <iostream>
using namespace std;
Player::Player(float startX, float startY, float maxSpeedX, float maxSpeedY) 
: 
Unit(startX, startY, maxSpeedX, maxSpeedY), 
m_animation(60,90) //storleken på varje bild i texturesheet
{
	m_sprite.setPosition(sf::Vector2f(startX, startY));
}

Player::Player(float startX, float startY, int width, int height, float maxSpeedX, float maxSpeedY) 
: 
Unit(startX, startY, width, height, maxSpeedX, maxSpeedY), 
m_animation(60,90)
{
	m_sprite.setPosition(sf::Vector2f(startX, startY));
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

void Player::updateAnimation(float deltaTime)
{
	if (m_speedX == 0)
	{
		if (m_state != idle)
		{
			m_animation.loop(0, 3, 0, 5/deltaTime);
			m_sprite.setScale(1, 1); //TEMP just nu bara titta åt ett håll idle
			m_state = idle;
		}
	}
	else if (m_speedX == m_maxSpeedX)
	{
		if (m_state != run)
		{
			m_animation.loop(4, 7, 2, 5/deltaTime);
			m_state = run;
		}
		m_sprite.setScale(1, 1);
	}
	else if (m_speedX == -m_maxSpeedX)
	{
		if (m_state != run)
		{
			m_animation.loop(4, 7, 2, 5/deltaTime);
			m_state = run;
		}
		m_sprite.setScale(-1, 1);
		m_sprite.setPosition(m_positionX + m_animation.getWidth(), m_positionY);
	}
	else if (0 < m_speedX)
	{
		if (m_state != walk)
		{
			m_animation.loop(0, 3, 1, 5/deltaTime);
			m_state = walk;
		}
		m_sprite.setScale(1, 1);
	}
	else if (m_speedX < 0)
	{
		if (m_state != walk)
		{
			m_animation.loop(0, 3, 1, 5/deltaTime);
			m_state = walk;
		}
		m_sprite.setScale(-1, 1);
		m_sprite.setPosition(m_positionX+m_animation.getWidth(), m_positionY);
	}
	m_sprite.setTextureRect(m_animation.getRectangle(deltaTime));
}