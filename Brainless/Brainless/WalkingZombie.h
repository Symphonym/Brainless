#ifndef _WALKING_ZOMBIE
#define _WALKING_ZOMBIE
#include "Zombie.h"

class WalkingZombie : public Zombie
{
public:
	WalkingZombie(sf::Vector2f startPosition, int maxLengthX);

	void updateTask(float deltaTime);
	void updateAnimation(float deltaTime);
	int getWalkLenght();

	enum AnimationState
	{
		anim_noAnimation,
		anim_walking
	};

	void wallLeft();
	void wallRight();

protected:

private:
	AnimationState m_animState;
	Direction m_spriteDirection;
	float m_maxPositionX;
	float m_minPositionX;
	int m_walkLenght;
	int m_currentDisparity;
	Direction m_direction;
};

#endif