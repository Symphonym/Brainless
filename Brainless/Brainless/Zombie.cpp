#include "Zombie.h"

Zombie::Zombie(sf::Vector2f startPosition, sf::Vector2f size, sf::Vector2f maxSpeed, sf::Vector2f spriteOffset, UnitType ID)
:
Unit(startPosition, size, maxSpeed, spriteOffset, ID)
{

}

bool Zombie::collide(Unit *unit)
{
	return unit->getCollisionRect().intersects(getCollisionRect());
}