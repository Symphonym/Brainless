#ifndef _ZOMBIE
#define _ZOMBIE
#include "Unit.h"

class Zombie :	public Unit
{
public:
	Zombie(sf::Vector2f startPosition);

	void updateAnimation(float deltaTime);

	enum AnimationState
	{
		noAnimation,
		idle
	};

	//ev. flytta till unit, och slutet av updateAnimation (samma för alla)
	enum Direction
	{
		noDirection,
		left,
		right
	};

protected:

private:
	AnimationState m_animState;
	Direction m_spriteDirection;
};

#endif