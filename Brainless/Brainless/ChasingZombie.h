#ifndef _CHASING_ZOMBIE
#define _CHASING_ZOMBIE
#include "Zombie.h"

class ChasingZombie : public Zombie
{
public:
	ChasingZombie(sf::Vector2f startPosition, int maxLengthX, Unit* target);

	void updateTask(float deltaTime);
	void updateAnimation(float deltaTime);
	int getWalkLenght();

	enum AnimationState
	{
		anim_noAnimation,
		anim_walking
	};

	enum ChasingState
	{
		chase_idle,
		chase_returnHome,
		chase_chase
	};


protected:

private:
	AnimationState m_animState;
	Direction m_spriteDirection;
	int m_maxWalkLenght;
	int m_currentLength;
	Direction m_direction;
	Unit* m_target;
	ChasingState m_chaseState;
};

#endif