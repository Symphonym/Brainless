#ifndef _UNIT
#define _UNIT

#include <SFML\Graphics.hpp>
#include "Animation.h"

//version 0.1
class Unit
{
public:
	Unit(float startX, float startY, float maxSpeedX, float maxSpeedY);
	Unit(float startX, float startY, int width, int height, float maxSpeedX, float maxSpeedY);

	/*
	Updates the collisionbox's position, speed, acceleration
	*/
	void updateMovement(float gravity, float deltaTime);

	/*
	set functions
	ev. kanske behöver annat "format"?
	*/
	void setStatus(/*ev. mer status information */ bool inAir);
	void setPosition(float posX, float posY);
	void setSpeed(float speedX, float speedY);
	void setAcceleration(float accelerationX, float accelerationY);

	//changes the maxSpeed for movement
	void setMaxSpeed(float maxSpeed);

	sf::Sprite getSprite();
	void setTexture(sf::Texture& texture);
	void draw();

	//updates animation
	virtual void updateAnimation(float deltaTime) = 0;

	//getters
	float getPositionX(){ return m_positionX; }
	float getPositionY(){ return m_positionY; }
	float getSpeedX(){ return m_speedX; }
	float getSpeedY(){ return m_speedY; }
	float getAccelerationX(){ return m_accelerationX; }
	float getAccelerationY(){ return m_accelerationY; }
	int getWidth(){ return m_width; }
	int getHeight(){ return m_height; }
	bool getInAir(){ return m_inAir; }
protected:
	float m_positionX;
	float m_positionY;
	float m_speedX;
	float m_speedY;
	float m_accelerationX;
	float m_accelerationY;
	float m_maxSpeedX;
	float m_maxSpeedY;
	int m_width;
	int m_height;
	sf::Sprite m_sprite;
	bool m_inAir;
	Animation m_animation;
private:
};



#endif
