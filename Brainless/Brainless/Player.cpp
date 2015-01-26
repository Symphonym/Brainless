/*



*/
#include "Player.h"
#include <iostream>
using namespace std;
Player::Player(float startX, float startY, float maxSpeedX, float maxSpeedY) : Unit(startX, startY, maxSpeedX, maxSpeedY)
{
	m_sprite.setPosition(sf::Vector2f(startX, startY));
}

Player::Player(float startX, float startY, int width, int height, float maxSpeedX, float maxSpeedY) : Unit(startX, startY, width, height, maxSpeedX, maxSpeedY)
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
			m_accelerationX = -m_speedX * 20;
		}
		else m_accelerationX = -200;
		if (-20 <= m_speedX && m_speedX <= 20)
		{
			cout << "AAAAAAAA" << endl;
			m_speedX -= 100;
		}
		slide = false;

	}
	//Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (m_speedX < -20)
		{
			m_accelerationX = -m_speedX * 20;
		}
		else m_accelerationX = 200;
		if (-20 <= m_speedX && m_speedX <= 20)
		{
			cout << "DDDDDDDD" << endl;
			m_speedX += 100;
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