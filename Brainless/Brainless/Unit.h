#ifndef _UNIT
#define _UNIT

#include <SFML\Graphics.hpp>
#include "Animation.h"
#include "SoundPlayer.h"
#include <vector>
#include <fstream>

//version 0.2
class Game;
class Item;
class Unit
{
public:
	enum UnitType
	{
		ID_Player,
		ID_IdleZombie,
		ID_WalkingZombie,
		ID_ChasingZombie
	};

	Unit(sf::Vector2f startPosition, sf::Vector2f size, sf::Vector2f maxSpeed, sf::Vector2f spriteOffset, UnitType ID);

	/*
	Updates the collisionbox's position, speed, acceleration
	*/
	void updateMovement(float gravity, float deltaTime);
	virtual void updateTask(float deltaTime) = 0;
	virtual void wallLeft();
	virtual void wallRight();

	virtual bool onInteractedWith(Item &otherItem, Game &game) { return false; } // Called when an item interacts with this unit, returning TRUE will destroy this unit
	virtual void onCollideWith(Unit *unit){};
	virtual void onCollideWithItem(Item &item) {};

	virtual void takesDamage(sf::Vector2f collisionPos){};

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	// Set player status
	void setInAir(bool inAir);
	void setTilt(bool inTilt);
	void setPosition(sf::Vector2f position);
	void setSpeed(sf::Vector2f speed);
	void setAcceleration(sf::Vector2f acceleration);

	//changes the maxSpeed for movement
	void setMaxSpeed(float maxSpeed);

	void setTexture(int index, sf::Texture& texture);
	void addTexture(sf::Texture& texture);
	void draw();

	bool isMovementEnabled() const;

	//updates animation
	virtual void updateAnimation(float deltaTime) = 0;
	sf::Sprite getSprite();

	enum Direction
	{
		dir_noDirection,
		dir_left,
		dir_right
	};

	enum RenderingModes
	{
		Above,
		Depth,
		Behind
	};

	void setRenderingMode(RenderingModes mode);

	
	sf::Vector2f getPosition() const;
	sf::Vector2f getSpeed() const;
	sf::Vector2f getAcceleration() const;
	sf::Vector2f getSize() const;
	bool getInAir() const;
	bool getInTilt() const;
	sf::FloatRect getCollisionRect();
	UnitType getUnitType();
	Direction getDirection();


	void collisionLeft(float posX, float speedX, float accX);
	void collisionRight(float posX, float speedX, float accX);
	void collisionUp(float posY, float speedY, float accY);
	void collisionDown(float posY, float speedY, float accY);
	bool updateCollision();

protected:
	RenderingModes m_renderingMode;
	bool m_isMovementEnabled;

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
	bool m_inTilt;
	Animation m_animation;
	void updateSpriteDirection();
	Direction m_spriteDirection;

	int m_collisionUp;
	int m_collisionLeft;
	int m_collisionRight;
	int m_collisionDown;
	sf::Vector2f m_collisionNewPos;
	sf::Vector2f m_collisionNewSpeed;
	sf::Vector2f m_collisionNewAcc;

	static Unit* s_playerPointer;
private:

	UnitType m_UnitID;
};



#endif
