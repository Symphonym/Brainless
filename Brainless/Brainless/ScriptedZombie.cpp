

#include "ScriptedZombie.h"


ScriptedZombie::ScriptedZombie(Zombie* baseZombie, int scriptID)
:
m_baseZombie(baseZombie),
m_scriptID(scriptID)
{

}

/*
Updates the collisionbox's position, speed, acceleration
*/
void ScriptedZombie::updateMovement(float gravity, float deltaTime)
{ 
	m_baseZombie->updateMovement(gravity, deltaTime);
}
void ScriptedZombie::updateTask(float deltaTime){ m_baseZombie->updateTask(deltaTime); }
void ScriptedZombie::wallLeft(){ m_baseZombie->wallLeft(); }
void ScriptedZombie::wallRight(){ m_baseZombie-> wallRight(); }

bool ScriptedZombie::onInteractedWith(Item &otherItem, Game &game){ return m_baseZombie->onInteractedWith(otherItem, game); }
void ScriptedZombie::onCollideWith(Unit *unit){ m_baseZombie->onCollideWith(unit); }
void ScriptedZombie::onCollideWithItem(Item &item){ m_baseZombie->onCollideWithItem(item); }

void ScriptedZombie::takesDamage(sf::Vector2f collisionPos, int damage){ m_baseZombie->takesDamage(collisionPos, damage); }

void ScriptedZombie::serialize(std::ofstream &writer) const //INTE KLART,
{ 

	m_baseZombie->serialize(writer); 
}
void ScriptedZombie::deserialize(std::ifstream &reader) //INTE KLART,
{ 
	m_baseZombie->deserialize(reader); 
}

int ScriptedZombie::getScriptID()
{
	return m_scriptID;
}

// Set player status
void ScriptedZombie::setInAir(bool inAir){ m_baseZombie->setInAir(inAir); }
void ScriptedZombie::setTilt(bool inTilt){ m_baseZombie->setTilt(inTilt); }
void ScriptedZombie::setPosition(sf::Vector2f position){ m_baseZombie->setPosition(position); }
void ScriptedZombie::setSpeed(sf::Vector2f speed){ m_baseZombie->setSpeed(speed); }
void ScriptedZombie::setAcceleration(sf::Vector2f acceleration){ m_baseZombie->setAcceleration(acceleration); }

//changes the maxSpeed for movement

void ScriptedZombie::setTexture(int index, sf::Texture& texture){ m_baseZombie->setTexture(index, texture); }
void ScriptedZombie::addTexture(sf::Texture& texture){ m_baseZombie->addTexture(texture); }
void ScriptedZombie::draw(){ m_baseZombie->draw(); }

bool ScriptedZombie::isMovementEnabled() const{ return m_baseZombie->isMovementEnabled(); }

//updates animation
void ScriptedZombie::updateAnimation(float deltaTime){ m_baseZombie->updateAnimation(deltaTime); }
sf::Sprite ScriptedZombie::getSprite(){ return m_baseZombie->getSprite(); }


void ScriptedZombie::setRenderingMode(RenderingModes mode){ m_baseZombie->setRenderingMode(mode); }


sf::Vector2f ScriptedZombie::getPosition() const{ return m_baseZombie->getPosition(); }
sf::Vector2f ScriptedZombie::getSpeed() const{ return m_baseZombie->getSpeed(); }
sf::Vector2f ScriptedZombie::getAcceleration() const{ return m_baseZombie->getAcceleration(); }
sf::Vector2f ScriptedZombie::getSize() const{ return m_baseZombie->getSize(); }
bool ScriptedZombie::getInAir() const{ return m_baseZombie->getInAir(); }
bool ScriptedZombie::getInTilt() const{ return m_baseZombie->getInTilt(); }
sf::FloatRect ScriptedZombie::getCollisionRect(){ return m_baseZombie->getCollisionRect(); }
Unit::UnitType ScriptedZombie::getUnitType(){ return m_baseZombie->getUnitType(); }
Unit::Direction ScriptedZombie::getDirection(){ return m_baseZombie->getDirection(); }


void ScriptedZombie::collisionLeft(float posX, float speedX, float accX){ m_baseZombie->collisionLeft(posX, speedX, accX); }
void ScriptedZombie::collisionRight(float posX, float speedX, float accX){ m_baseZombie->collisionRight(posX, speedX, accX); }
void ScriptedZombie::collisionUp(float posY, float speedY, float accY){ m_baseZombie->collisionUp(posY, speedY, accY); }
void ScriptedZombie::collisionDown(float posY, float speedY, float accY){ m_baseZombie->collisionDown(posY, speedY, accY); }
bool ScriptedZombie::updateCollision(){ return m_baseZombie->updateCollision(); }