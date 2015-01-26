/*



*/
#include "Player.h"

Player::Player(float startX, float startY, float maxSpeedX, float maxSpeedY) : Unit(startX,startY, maxSpeedX, maxSpeedY)
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
		m_accelerationX = -100;
		if (m_speedX == 0)m_speedX -= 50;
		slide = false;

	}
	//Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_accelerationX = 100;
		if (m_speedX == 0) m_speedX += 50;
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
		else m_accelerationX = -m_speedX * 3;
	}
	//Jump
	if (!m_inAir && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		m_speedY = -300;
		m_inAir = true;
	}
}