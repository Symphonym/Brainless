#include "Zombie.h"
#include "Item.h"
#include "Game.h"

Zombie::Zombie(sf::Vector2f startPosition, sf::Vector2f size, sf::Vector2f maxSpeed, sf::Vector2f spriteOffset, UnitType ID)
:
Unit(startPosition, size, maxSpeed, spriteOffset, ID)
{

}

bool Zombie::onInteractedWith(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Brain")
	{
		game.addSavedZombie(1);
		// TODO add brain giving logic
		return true;
	}
}

bool Zombie::collide(Unit *unit)
{
	return unit->getCollisionRect().intersects(getCollisionRect());
}