#ifndef _UNIT
#define _UNIT

#include <SFML\Graphics.hpp>
#include "Animation.h"
#include <vector>

//version 0.2
class Unit
{
public:
	Unit(sf::Vector2f startPosition, sf::Vector2f size, sf::Vector2f maxSpeed, sf::Vector2f spriteOffset);

	/*
	Updates the collisionbox's position, speed, acceleration
	*/
	void updateMovement(float gravity, float deltaTime);
	virtual void updateTask(float deltaTime) = 0;

	// Set player status
	void setStatus(bool inAir);
	void setPosition(sf::Vector2f position);
	void setSpeed(sf::Vector2f speed);
	void setAcceleration(sf::Vector2f acceleration);

	//changes the maxSpeed for movement
	void setMaxSpeed(float maxSpeed);

	void setTexture(int index, sf::Texture& texture);
	void addTexture(sf::Texture& texture);
	void draw();

	//updates animation
	virtual void updateAnimation(float deltaTime) = 0;
	sf::Sprite getSprite();

	enum Direction
	{
		noDirection,
		left,
		right
	};


	sf::Vector2f getPosition() const;
	sf::Vector2f getSpeed() const;
	sf::Vector2f getAcceleration() const;
	sf::Vector2f getSize() const;
	bool getInAir() const;
	sf::FloatRect getCollisionRect();

protected:
	sf::Vector2f m_position;
	sf::Vector2f m_speed;
	sf::Vector2f m_acceleration;
	sf::Vector2f m_maxSpeed;
	sf::Vector2f m_size;

	typedef std::vector<sf::Sprite> SpriteVector;
	SpriteVector m_spriteSheets;
	sf::Sprite* m_sprite;
	sf::Vector2f m_spriteOffset;

	bool m_specialSpriteDirection; //if true, don't calculate a direction, use the current set direction
	bool m_inAir;
	Animation m_animation;
	void updateSpriteDirection();
	Direction m_spriteDirection;
private:
};



#endif
