#ifndef _ZOMBIE
#define _ZOMBIE
#include "Unit.h"

class IdleZombie : public Unit
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