#ifndef _IDLE_ZOMBIE
#define _IDLE_ZOMBIE
#include "Zombie.h"

class IdleZombie : public Zombie
{
public:
	IdleZombie(sf::Vector2f startPosition, Direction startDirection, int Texture);

	void updateTask(float deltaTime);
	void updateAnimation(float deltaTime);

	enum AnimationState
	{
		anim_noAnimation,
		anim_idle
	};

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

protected:

private:
	AnimationState m_animState;
};

#endif