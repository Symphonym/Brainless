#ifndef _ZOMBIE
#define _ZOMBIE
#include "Unit.h"

class Zombie : public Unit
{
public:
	Zombie(sf::Vector2f startPosition, sf::Vector2f size, sf::Vector2f maxSpeed, sf::Vector2f spriteOffset, UnitType ID);

	virtual bool onInteractedWith(Item &otherItem, Game &game);

	void flash(sf::Vector2f cameraPos);

	bool collide(Unit *unit);
protected:

private:
	float m_flashAlpha;
};

#endif