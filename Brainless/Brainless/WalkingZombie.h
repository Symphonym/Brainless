#ifndef _WALKING_ZOMBIE
#define _WALKING_ZOMBIE
#include "Zombie.h"

class WalkingZombie : public Zombie
{
public:
	WalkingZombie(sf::Vector2f startPosition, int maxLengthX);

	void updateTask(float deltaTime);
	void updateAnimation(float deltaTime);

	enum AnimationState
	{
		noAnimation,
		idle
	};


protected:

private:
	AnimationState m_animState;
	Direction m_spriteDirection;
	int m_maxPositionX;
	int m_minPositionX;
	int m_currentDisparity;
	Direction m_direction;
};

#endif