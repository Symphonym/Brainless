#ifndef _IDLE_ZOMBIE
#define _IDLE_ZOMBIE
#include "Zombie.h"

class IdleZombie : public Zombie
{
public:
	IdleZombie(sf::Vector2f startPosition,Direction startDirection);

	void updateTask(float deltaTime);
	void updateAnimation(float deltaTime);

	enum AnimationState
	{
		anim_noAnimation,
		anim_idle
	};

protected:

private:
	AnimationState m_animState;
};

#endif