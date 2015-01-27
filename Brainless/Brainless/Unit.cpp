#include "Unit.h"
#include "Renderer.h"
#include <iostream>
using namespace std;
//V0.01
Unit::Unit(float startX, float startY, float maxSpeedX, float maxSpeedY) 
: 
m_positionX(startX), m_positionY(startY), 
m_speedX(0), m_speedY(0), 
m_accelerationX(0),m_accelerationY(0), 
m_width(200), m_height(200), 
m_maxSpeedX(maxSpeedX), m_maxSpeedY(maxSpeedY), 
m_inAir(false)
{
	m_sprite.setPosition(sf::Vector2f(startX, startY));
}
Unit::Unit(float startX, float startY, int width, int height, float maxSpeedX, float maxSpeedY) 
: 
m_positionX(startX), m_positionY(startY), 
m_speedX(0), m_speedY(0),
m_accelerationX(0), m_accelerationY(0), m_inAir(false), 
m_width(width), m_height(height), 
m_maxSpeedX(maxSpeedX), m_maxSpeedY(maxSpeedY)
{
	m_sprite.setPosition(sf::Vector2f(startX, startY));
}
void Unit::updateMovement(float gravity, float deltaTime)
{
	// kanske inte för allt? Exempel spöken om de ska baseras på samma klass.
	if (m_inAir) m_accelerationY = gravity;
	else
	{
		m_speedY = 0;
		m_accelerationY = 0;
	}


	m_speedX += m_accelerationX * deltaTime;
	m_speedY += m_accelerationY * deltaTime;
	if (m_speedX > m_maxSpeedX) m_speedX = m_maxSpeedX;
	if (m_speedY > m_maxSpeedY) m_speedY = m_maxSpeedY;
	if (m_speedX < -m_maxSpeedX) m_speedX = -m_maxSpeedX;
	if (m_speedY < -m_maxSpeedY) m_speedY = -m_maxSpeedY;
	m_positionX += m_speedX * deltaTime;
	m_positionY += m_speedY * deltaTime;

	m_sprite.setPosition(sf::Vector2f(m_positionX, m_positionY)); //temp


	//output en gång per sekund
	static float timeTest = 0;
	timeTest += deltaTime;
	if (timeTest >= 1)
	{
		timeTest = 0;
		cout << "m_posX: " << m_positionX << endl;
		cout << "m_posY: " << m_positionY << endl;
		cout << "m_speedX: " << m_speedX << endl;
		cout << "m_speedY: " << m_speedY << endl;
		cout << "m_accelX: " << m_accelerationX << endl;
		cout << "m_accelY: " << m_accelerationY << endl;
		cout << "inAir: " << m_inAir << endl;
		cout << "deltaTime: " << deltaTime << endl;
	}
}

void Unit::setStatus(bool inAir)
{
	m_inAir = inAir;
}
void Unit::setPosition(float posX, float posY)
{
	m_positionX = posX;
	m_positionY = posY;

}
void Unit::setSpeed(float speedX, float speedY)
{
	m_speedX = speedX;
	m_speedY = speedY;
}
void Unit::setAcceleration(float accelerationX, float accelerationY)
{
	m_accelerationX = accelerationX;
	m_accelerationY = accelerationY;
}

sf::Sprite Unit::getSprite()
{
	return m_sprite;
}

void Unit::setTexture(sf::Texture& texture)
{
	m_sprite.setTexture(texture);
}

void Unit::draw()
{
	Renderer::instance().draw(m_sprite);
}

