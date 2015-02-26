#include "Unit.h"
#include "Renderer.h"
#include "ConversationBox.h"
#include <iostream>
#define SPRITESIZE 256 //storleken på varje bild i texturesheet

Unit* Unit::s_playerPointer = nullptr;

//V0.02
Unit::Unit(sf::Vector2f startPosition, sf::Vector2f size, sf::Vector2f maxSpeed, sf::Vector2f spriteOffset, UnitType ID)
:
m_renderingMode(RenderingModes::Depth),
m_isMovementEnabled(true),
m_position(startPosition),
m_speed(sf::Vector2f(0, 0)),
m_acceleration(sf::Vector2f(0, 0)),
m_size(size),
m_maxSpeed(maxSpeed),
m_inAir(false),
m_inTilt(false),
m_animation(SPRITESIZE, SPRITESIZE),
m_spriteOffset(spriteOffset),
m_spriteDirection(dir_right),
m_specialSpriteDirection(false),
m_UnitID(ID)
{

}
void Unit::updateMovement(float gravity, float deltaTime)
{
	if (!m_isMovementEnabled)
		return;

	float accelYtrue = 0;
	//normal inAir
	if (m_inAir && !m_inTilt)
	{

		accelYtrue = m_acceleration.y + gravity;
	}
	//hotfix for faulty collision when inAir and tiltwalking
	else if (m_inAir && m_inTilt)
	{
		m_position.y += abs(m_speed.x) * deltaTime ;
	}
	//normal onGround
	else
	{
		m_speed.y = 0;
		m_acceleration.y = 0;
	}

	m_speed.x += m_acceleration.x * deltaTime;
	m_speed.y += accelYtrue * deltaTime;
	if (m_speed.x > m_maxSpeed.x) m_speed.x = m_maxSpeed.x;
	if (m_speed.y > m_maxSpeed.y) m_speed.y = m_maxSpeed.y;
	if (m_speed.x < -m_maxSpeed.x) m_speed.x = -m_maxSpeed.x;
	if (m_speed.y < -m_maxSpeed.y) m_speed.y = -m_maxSpeed.y;
	m_position.x += m_speed.x * deltaTime;
	m_position.y += m_speed.y * deltaTime;
}

void Unit::wallLeft()
{

}

void Unit::wallRight()
{

}

void Unit::serialize(std::ofstream &writer) const
{
	// This is so the reader can quickly know what unit it is
	writer << static_cast<int>(m_UnitID) << std::endl;

	writer << m_position.x << std::endl;
	writer << m_position.y << std::endl;
	writer << m_speed.x << std::endl;
	writer << m_speed.y << std::endl;
	writer << m_acceleration.x << std::endl;
	writer << m_acceleration.y << std::endl;
	writer << m_size.x << std::endl;
	writer << m_size.y << std::endl;
	writer << m_specialSpriteDirection << std::endl;
	writer << m_inAir << std::endl;
	writer << m_inTilt << std::endl;
	writer << static_cast<int>(m_spriteDirection) << std::endl;
	writer << static_cast<int>(m_renderingMode) << std::endl;
}
void Unit::deserialize(std::ifstream &reader)
{
	// ID is read initially outside this function to initially create the unit
	
	reader >> m_position.x >> m_position.y;
	reader >> m_speed.x >> m_speed.y;
	reader >> m_acceleration.x >> m_acceleration.y;
	reader >> m_size.x >> m_size.y;
	reader >> m_specialSpriteDirection;
	reader >> m_inAir;
	reader >> m_inTilt;

	int directionType = 0;
	reader >> directionType;
	m_spriteDirection = static_cast<Direction>(directionType);

	int renderingType = 0;
	reader >> renderingType;
	m_renderingMode = static_cast<RenderingModes>(renderingType);
}

void Unit::setInAir(bool inAir)
{
	m_inAir = inAir;
}
void Unit::setTilt(bool inTilt)
{
	m_inTilt = inTilt;
}
void Unit::setPosition(sf::Vector2f position)
{
	m_position = position;
}
void Unit::setSpeed(sf::Vector2f speed)
{
	m_speed = speed;
}
void Unit::setAcceleration(sf::Vector2f acceleration)
{
	m_acceleration = acceleration;
}
void Unit::setRenderingMode(RenderingModes mode)
{
	m_renderingMode = mode;
}

sf::Sprite Unit::getSprite()
{
	return (*m_sprite);
}

bool Unit::isMovementEnabled() const
{
	return m_isMovementEnabled;
}

void Unit::setTexture(int index, sf::Texture& texture)
{
	m_sprite->setTexture(texture);
	//error sheet.size() < 1
	//error index < 0
	if (index < m_spriteSheets.size())
	{
		m_spriteSheets[index].setTexture(texture);
	}
}

void Unit::addTexture(sf::Texture& texture)
{
	m_spriteSheets.push_back(sf::Sprite(texture));
	m_sprite = &m_spriteSheets[0];
}

void Unit::draw()
{
	if (m_renderingMode == RenderingModes::Above)
		Renderer::instance().drawAbove((*m_sprite));
	else if (m_renderingMode == RenderingModes::Depth)
	Renderer::instance().drawDepth((*m_sprite));
	else if (m_renderingMode == RenderingModes::Behind)
		Renderer::instance().drawBehind((*m_sprite));
}

void Unit::updateSpriteDirection()
{
	if (!m_specialSpriteDirection)
	{
		//Sprite mirroring and offset.
		if (m_spriteDirection == dir_left)
		{
			if (0 < m_speed.x)
			{
				m_spriteDirection = dir_right;
				m_sprite->setScale(1, 1);
				m_sprite->setPosition(sf::Vector2f(m_position.x + m_spriteOffset.x, m_position.y + m_spriteOffset.y));

			}
			m_sprite->setScale(-1, 1);
			m_sprite->setPosition(m_position.x + m_spriteOffset.x + m_animation.getWidth(), m_position.y + m_spriteOffset.y);
		}
		else if (m_spriteDirection == dir_right)
		{
			if (m_speed.x < 0)
			{
				m_spriteDirection = dir_left;
				m_sprite->setScale(-1, 1);
				m_sprite->setPosition(m_position.x + m_spriteOffset.x + m_animation.getWidth(), m_position.y + m_spriteOffset.y);
			}
			m_sprite->setScale(1, 1);
			m_sprite->setPosition(sf::Vector2f(m_position.x + m_spriteOffset.x, m_position.y + m_spriteOffset.y));
		}
		else
		{
			m_sprite->setScale(1, 1);
			m_sprite->setPosition(sf::Vector2f(m_position.x + m_spriteOffset.x, m_position.y + m_spriteOffset.y));
		}
	}
	else
	{
		if (m_spriteDirection == dir_left)
		{
			m_sprite->setScale(-1, 1);
			m_sprite->setPosition(m_position.x + m_spriteOffset.x + m_animation.getWidth(), m_position.y + m_spriteOffset.y);
		}
		else if (m_spriteDirection == dir_right)
		{
			m_sprite->setScale(1, 1);
			m_sprite->setPosition(sf::Vector2f(m_position.x + m_spriteOffset.x, m_position.y + m_spriteOffset.y));
		}
		else
		{
			m_sprite->setScale(1, 1);
			m_sprite->setPosition(sf::Vector2f(m_position.x + m_spriteOffset.x, m_position.y + m_spriteOffset.y));
		}
	}

}

bool Unit::getInAir() const
{
	return m_inAir;
}
bool Unit::getInTilt() const
{
	return m_inTilt;
}
sf::Vector2f Unit::getPosition() const
{
	return m_position;
}
sf::Vector2f Unit::getSpeed() const
{
	return m_speed;
}
sf::Vector2f Unit::getAcceleration() const
{
	return m_acceleration;
}
sf::Vector2f Unit::getSize() const
{ 
	return m_size;
}
sf::FloatRect Unit::getCollisionRect()
{
	return sf::FloatRect(m_position, m_size);
}

Unit::UnitType Unit::getUnitType()
{
	return m_UnitID;
}

Unit::Direction Unit::getDirection()
{
	return m_spriteDirection;
}


void Unit::collisionLeft(float posX, float speedX, float accX)
{
	m_collisionLeft++;
	m_collisionNewPos.x = posX;
	m_collisionNewSpeed.x = speedX;
	m_collisionNewAcc.x = accX;
}


void Unit::collisionRight(float posX, float speedX, float accX)
{
	m_collisionRight++;
	m_collisionNewPos.x = posX;
	m_collisionNewSpeed.x = speedX;
	m_collisionNewAcc.x = accX;
}

void Unit::collisionUp(float posY, float speedY, float accY)
{
	m_collisionUp++;
	m_collisionNewPos.y = posY;
	m_collisionNewSpeed.y = speedY;
	m_collisionNewAcc.y = accY;
}
void Unit::collisionDown(float posY, float speedY, float accY)
{
	m_collisionDown++;
	m_collisionNewPos.y = posY;
	m_collisionNewSpeed.y = speedY;
	m_collisionNewAcc.y = accY;
}

bool Unit::updateCollision()
{
	//massa if
	if (m_collisionDown == 0 &&
		m_collisionLeft == 0 &&
		m_collisionRight == 0 &&
		m_collisionUp == 0) return false;
	//debug typ
	if (m_collisionLeft > 0 && m_collisionRight > 0)
		std::cout << "OM DU INTE ÄR INUTI I VÄGGEN NÄR DETTA KOMMER UPP, UTÖKA FÖR VÄNSTER/HÖGER KROCK SAMTIDIGT" << std::endl;
	if (m_collisionUp > 0 && m_collisionDown > 0)
		std::cout << "OM DU INTE ÄR INUTI I VÄGGEN NÄR DETTA KOMMER UPP, UTÖKA FÖR UP/NERE KROCK SAMTIDIGT" << std::endl;

	/*
	med följande värden och nuvarande tile kollision i level 2015-02-24
	COLLISION_WIDTH 80
	COLLISION_HEIGHT 190
	unitLedgeOffset 20

	kollision nere och höger
	0201
	0301

	kollision nere och vänster
	3001
	2001

	kollision uppe och höger
	0310
	0210

	kollision uppe och vänster
	3010
	2010

	kollision nere kan hända 
	0002
	0001
	1001

	uppe kan hända
	0110
	0020
	0010

	höger kan hända
	0300
	0200
	0100

	vänster kan hända
	3000
	2000
	1000
	
	//EJ "LÄNKADE" FUNNA KOMBINATIONER
	0202 , har uppstått, inget fel syntes. Om det hände vid nere, inget fel. Dock vet ej exakt när det inträffade.

	Kommentera bort cout för att se kombinationer som uppstår

	*/

	//std::cout << m_collisionLeft << m_collisionRight << m_collisionUp << m_collisionDown << std::endl;

	//NY VERSION TILL OVAN SIFFROR, EJ MYCKET TESTAT.
	//Collision nere
	if (m_collisionDown > 0 && m_collisionLeft <= m_collisionDown && m_collisionRight <= m_collisionDown)
	{
		m_position.y = m_collisionNewPos.y;
		m_speed.y = m_collisionNewSpeed.y;
		m_acceleration.y = m_collisionNewAcc.y;
		m_inAir = false;
		m_inTilt = false;
	}
	//Collision ovan
	else if (m_collisionUp > 0 && m_collisionLeft <= m_collisionUp && m_collisionRight <= m_collisionUp)
	{
		m_position.y = m_collisionNewPos.y;
		m_speed.y = m_collisionNewSpeed.y;
		m_acceleration.y = m_collisionNewAcc.y;
	}
	//Collision vänster +
	else if (m_collisionLeft > 0)
	{
		//vänster och nere
		if (m_collisionDown > 0)
		{
			m_position = m_collisionNewPos;
			m_speed = m_collisionNewSpeed;
			m_acceleration = m_collisionNewAcc;
			m_inAir = false;
			m_inTilt = false;
			wallLeft();
		}
		//vänster och uppe
		else if (m_collisionUp > 0)
		{
			m_position = m_collisionNewPos;
			m_speed = m_collisionNewSpeed;
			m_acceleration = m_collisionNewAcc;
			wallLeft();
		}
		//bara vänster
		else
		{
			m_position.x = m_collisionNewPos.x;
			m_speed.x = m_collisionNewSpeed.x;
			m_acceleration.x = m_collisionNewAcc.x;
			wallLeft();
		}
	}
	//Collision höger +
	else if (m_collisionRight > 0)
	{
		//höger och nere
		if (m_collisionDown > 0)
		{
			m_position = m_collisionNewPos;
			m_speed = m_collisionNewSpeed;
			m_acceleration = m_collisionNewAcc;
			m_inAir = false;
			m_inTilt = false;
			wallRight();
		}
		//höger och uppe
		else if (m_collisionUp > 0)
		{
			m_position = m_collisionNewPos;
			m_speed = m_collisionNewSpeed;
			m_acceleration = m_collisionNewAcc;
			wallRight();
		}
		//bara höger
		else
		{
			m_position.x = m_collisionNewPos.x;
			m_speed.x = m_collisionNewSpeed.x;
			m_acceleration.x = m_collisionNewAcc.x;
			wallRight();
		}
	}
	
	m_collisionNewPos = m_position;
	m_collisionNewSpeed = m_speed;
	m_collisionNewAcc = m_acceleration;
	m_collisionLeft = 0;
	m_collisionRight = 0;
	m_collisionUp = 0;
	m_collisionDown = 0;
	return true;
}