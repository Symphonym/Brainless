#ifndef _IDLE_ZOMBIE
#define _IDLE_ZOMBIE
#include "Zombie.h"

class IdleZombie : public Zombie
{
public:
	IdleZombie(sf::Vector2f startPosition);

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
};

#endif