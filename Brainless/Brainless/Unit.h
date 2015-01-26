#ifndef _UNIT
#define _UNIT

#include <SFML\Graphics.hpp>
//version 0.1
class Unit
{
public:
	Unit(float startX, float startY);
	Unit(float startX, float startY, int width, int height);
	
	/*
		updaterar collisionsboxens position
	*/
	void updateMovement(float gravity);

	/*
		sätter status och collisionxbosens position 
	*/
	void setStatusAndPosition(/*ev. mer status information */ bool inAir, float posX, float posY, float speedX, float speedY, float accelX, float accelY);

	sf::Sprite getSprite();
	void setTexture(sf::Texture& texture);
	void draw();
	float getPositionX(){ return m_positionX; }
	float getPositionY(){ return m_positionY; }
	float getSpeedX(){ return m_positionY; }
	float getSpeedY(){ return m_positionY; }
	float getAccelerationX(){ return m_positionY; }
	float getAccelerationY(){ return m_positionY; }
	float getWidth(){ return m_width; }
	float getHeight(){ return m_height; }
protected:
	float m_positionX;
	float m_positionY;
	float m_speedX;
	float m_speedY;
	float m_accelerationX;
	float m_accelerationY;
	int m_width;
	int m_height;
	sf::Sprite m_sprite;
	bool inAir;
private:
};



#endif
