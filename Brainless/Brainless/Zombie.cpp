#include "Zombie.h"

Zombie::Zombie(sf::Vector2f startPosition, sf::Vector2f size, sf::Vector2f maxSpeed, sf::Vector2f spriteOffset)
:
Unit(startPosition, size, maxSpeed, spriteOffset)
{

}

bool Zombie::collide(Unit *unit)
{

}