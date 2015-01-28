#include "Unit.h"
#include "Renderer.h"
#include <iostream>
#include "ResourceLoader.h" //temp för felsök text

using namespace std;
//V0.01
Unit::Unit(float startX, float startY, float maxSpeedX, float maxSpeedY)
:
m_positionX(startX), m_positionY(startY),
m_speedX(0), m_speedY(0),
m_accelerationX(0), m_accelerationY(0),
m_width(60), m_height(90),
m_maxSpeedX(maxSpeedX), m_maxSpeedY(maxSpeedY),
m_inAir(false),
m_animation(60, 90) //storleken på varje bild i texturesheet
{
	m_sprite.setPosition(sf::Vector2f(startX, startY));
}
Unit::Unit(float startX, float startY, int width, int height, float maxSpeedX, float maxSpeedY)
:
m_positionX(startX), m_positionY(startY),
m_speedX(0), m_speedY(0),
m_accelerationX(0), m_accelerationY(0), m_inAir(false),
m_width(width), m_height(height),
m_maxSpeedX(maxSpeedX), m_maxSpeedY(maxSpeedY),
m_animation(60, 90) //storleken på varje bild i texturesheet
{
	m_sprite.setPosition(sf::Vector2f(startX, startY));

	text.setFont(ResourceLoader::instance().retrieveFont("EditorFont"));
	text.setCharacterSize(16);
	text2.setFont(ResourceLoader::instance().retrieveFont("EditorFont"));
	text2.setCharacterSize(16);
	text3.setFont(ResourceLoader::instance().retrieveFont("EditorFont"));
	text3.setCharacterSize(16);
	text4.setFont(ResourceLoader::instance().retrieveFont("EditorFont"));
	text4.setCharacterSize(16);

}
void Unit::updateMovement(float gravity, float deltaTime)
{
	float accelYtrue;
	if (m_inAir)
	{
		text.setPosition(0, 0);
		text.setString("inAir");
		text.setColor(sf::Color::Green);
		Renderer::instance().drawHUD(text);
	
		accelYtrue = m_accelerationY + gravity; // funkar inte fixa asap efter lunch
	}
	else
	{
		m_speedY = 0;
		m_accelerationY = 0;
		accelYtrue = 0;
	}


	m_speedX += m_accelerationX * deltaTime;
	m_speedY += accelYtrue * deltaTime;
	if (m_speedX > m_maxSpeedX) m_speedX = m_maxSpeedX;
	if (m_speedY > m_maxSpeedY) m_speedY = m_maxSpeedY;
	if (m_speedX < -m_maxSpeedX) m_speedX = -m_maxSpeedX;
	if (m_speedY < -m_maxSpeedY) m_speedY = -m_maxSpeedY;
	m_positionX += m_speedX * deltaTime;
	m_positionY += m_speedY * deltaTime;

	m_sprite.setPosition(sf::Vector2f(m_positionX, m_positionY)); //temp ska inte baseras på "collisionvärden"

	text2.setPosition(0, 20);
	text2.setString("speedY " + to_string(m_speedY));
	text2.setColor(sf::Color::Green);
	Renderer::instance().drawHUD(text2);
	text3.setPosition(0, 40);
	text3.setString("accelY " + to_string(m_accelerationY));
	text3.setColor(sf::Color::Green);
	Renderer::instance().drawHUD(text3);
	text4.setPosition(0, 60);
	text4.setString("accelY+grav " + to_string(accelYtrue));
	text4.setColor(sf::Color::Green);
	Renderer::instance().drawHUD(text4);

	////output en gång per sekund
	//static float timeTest = 0;
	//timeTest += deltaTime;
	//if (timeTest >= 1)
	//{
	//	timeTest = 0;
	//	cout << "m_posX: " << m_positionX << endl;
	//	cout << "m_posY: " << m_positionY << endl;
	//	cout << "m_speedX: " << m_speedX << endl;
	//	cout << "m_speedY: " << m_speedY << endl;
	//	cout << "m_accelX: " << m_accelerationX << endl;
	//	cout << "m_accelY: " << m_accelerationY << endl;
	//	cout << "m_accelYwithGrav: " << accelYtrue << endl;
	//	cout << "inAir: " << m_inAir << endl;
	//	cout << "deltaTime: " << deltaTime << endl;
	//}
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
	Renderer::instance().drawDepth(m_sprite);
}

sf::FloatRect Unit::getCollisionRect()
{
	return sf::FloatRect(m_positionX, m_positionY, m_width, m_height);
}