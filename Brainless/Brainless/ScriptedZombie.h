#ifndef _SCRIPTED_ZOMBIE
#define _SCRIPTED_ZOMBIE

#include "Unit.h"
#include "Zombie.h"
#include "Level.h"

class ScriptedZombie : public Unit
{
public:


	ScriptedZombie(Zombie* baseZombie, int scriptID);
//	ScriptedZombie(Zombie* baseZombie, int scriptID, std::vector<Level::ItemPtr> itemList); //MEMORY0

	/*
	Updates the collisionbox's position, speed, acceleration
	*/
	virtual ~ScriptedZombie();

	void updateMovement(float gravity, float deltaTime);
	void updateTask(float deltaTime);
	void wallLeft() override;
	void wallRight() override;

	bool onInteractedWith(Item &otherItem, Game &game);
	void onCollideWith(Unit *unit);
	void onCollideWithItem(Item &item);

	void takesDamage(sf::Vector2f collisionPos, int damage = 1);

	void serialize(std::ofstream &writer) const;
	void deserialize(std::ifstream &reader);
	int getScriptID();

	// Set player status
	void setInAir(bool inAir);
	void setTilt(bool inTilt);
	void setPosition(sf::Vector2f position);
	void setSpeed(sf::Vector2f speed);
	void setAcceleration(sf::Vector2f acceleration);

	//changes the maxSpeed for movement

	void setTexture(int index, sf::Texture& texture);
	void addTexture(sf::Texture& texture);
	void draw();

	bool isMovementEnabled() const;

	//updates animation
	virtual void updateAnimation(float deltaTime);
	sf::Sprite getSprite();


	void setRenderingMode(RenderingModes mode);


	sf::Vector2f getPosition() const;
	sf::Vector2f getSpeed() const;
	sf::Vector2f getAcceleration() const;
	sf::Vector2f getSize() const;
	bool getInAir() const;
	bool getInTilt() const;
	sf::FloatRect getCollisionRect();
	UnitType getUnitType();
	UnitType getRealUnitType();
	Direction getDirection();


	void collisionLeft(float posX, float speedX, float accX);
	void collisionRight(float posX, float speedX, float accX);
	void collisionUp(float posY, float speedY, float accY);
	void collisionDown(float posY, float speedY, float accY);
	bool updateCollision();

	int getWalkLength();
	int getTextureID();
	void incrementTexture();
private:
	int m_scriptID;
	Zombie* m_baseZombie;
	//std::vector<Level::ItemPtr> m_itemList; //MEMORY0

};



#endif
