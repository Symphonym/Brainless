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
		//normals
		ID_Player,
		ID_IdleZombie,
		ID_WalkingZombie,
		ID_ChasingZombie,
		ID_NONE,
		ID_ScriptZombie,
	
		//scripted
		ID_CabinetZombie
	};

	Unit(sf::Vector2f startPosition, sf::Vector2f size, sf::Vector2f maxSpeed, sf::Vector2f spriteOffset, UnitType ID);
	Unit(){}; //för scriptZombien
	virtual ~Unit(){};
	/*
	Updates the collisionbox's position, speed, acceleration
	*/
	virtual void updateMovement(float gravity, float deltaTime);
	virtual void updateTask(float deltaTime) = 0;
	virtual void wallLeft();
	virtual void wallRight();

	virtual bool onInteractedWith(Item &otherItem, Game &game) { return false; } // Called when an item interacts with this unit, returning TRUE will destroy this unit
	virtual void onCollideWith(Unit *unit){};
	virtual void onCollideWithItem(Item &item) {};

	virtual void takesDamage(sf::Vector2f collisionPos, int damage = 1){};

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);
	virtual int getScriptID(){ return -1; }

	// Set player status
	virtual void setInAir(bool inAir);
	virtual void setTilt(bool inTilt);
	virtual void setPosition(sf::Vector2f position);
	virtual	void setSpeed(sf::Vector2f speed);
	virtual	void setAcceleration(sf::Vector2f acceleration);

	virtual	void setTexture(int index, sf::Texture& texture);
	virtual	void addTexture(sf::Texture& texture);
	virtual	void draw();

	virtual	bool isMovementEnabled() const;

	//updates animation
	virtual void updateAnimation(float deltaTime) = 0;
	virtual	sf::Sprite getSprite();

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

	virtual	void setRenderingMode(RenderingModes mode);

	
	virtual	sf::Vector2f getPosition() const;
	virtual	sf::Vector2f getSpeed() const;
	virtual	sf::Vector2f getAcceleration() const;
	virtual	sf::Vector2f getSize() const;
	virtual	bool getInAir() const;
	virtual	bool getInTilt() const;
	virtual	sf::FloatRect getCollisionRect();
	virtual	UnitType getUnitType();
	virtual UnitType getRealUnitType();
	virtual	Direction getDirection();


	virtual	void collisionLeft(float posX, float speedX, float accX);
	virtual	void collisionRight(float posX, float speedX, float accX);
	virtual	void collisionUp(float posY, float speedY, float accY);
	virtual	void collisionDown(float posY, float speedY, float accY);
	virtual	bool updateCollision();

	//egentligen zombie, men var lättare att bara lägga dem i unit
	virtual int getWalkLength(){ return 0; }
	virtual int getTextureID(){ return 0; }
	virtual void incrementTexture(){};
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

	UnitType m_UnitID;
private:

};



#endif
